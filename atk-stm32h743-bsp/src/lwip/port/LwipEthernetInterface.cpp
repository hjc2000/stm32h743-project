#include "LwipEthernetInterface.h"
#include <base/container/Dictionary.h>
#include <base/container/List.h>
#include <base/di/SingletonGetter.h>
#include <base/stream/ReadOnlySpan.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <FreeRTOS.h>
#include <hal.h>
#include <lwip/dhcp.h>
#include <lwip/init.h>
#include <lwip/mem.h>
#include <lwip/memp.h>
#include <lwip/tcpip.h>
#include <lwip/timeouts.h>
#include <netif/etharp.h>
#include <stdint.h>
#include <stdio.h>
#include <task.h>

bsp::LwipEthernetInterface::LwipEthernetInterface()
{
	/* 默认本地IP为:192.168.1.30 */
	_ip_address[0] = 192;
	_ip_address[1] = 168;
	_ip_address[2] = 1;
	_ip_address[3] = 30;

	/* 默认子网掩码:255.255.255.0 */
	_netmask[0] = 255;
	_netmask[1] = 255;
	_netmask[2] = 255;
	_netmask[3] = 0;

	/* 默认网关:192.168.1.1 */
	_gateway[0] = 192;
	_gateway[1] = 168;
	_gateway[2] = 1;
	_gateway[3] = 1;
}

void bsp::LwipEthernetInterface::AddDefaultNetInterface()
{
	auto initialization_callback = [](netif *p) -> err_t
	{
		bsp::LwipEthernetInterface::Instance().InitializingNetifCallbackFunc();
		return err_enum_t::ERR_OK;
	};

	netif *netif_add_result = netif_add(&_lwip_netif,
										reinterpret_cast<ip_addr_t const *>(&_ip_address),
										reinterpret_cast<ip_addr_t const *>(&_netmask),
										reinterpret_cast<ip_addr_t const *>(&_gateway),
										nullptr,
										initialization_callback,
										tcpip_input);

	if (netif_add_result == nullptr)
	{
		DI_Console().WriteLine("添加网卡失败。");
		throw std::runtime_error{"添加网卡失败。"};
	}

	// 因为本函数是 “添加默认网卡” ，所以添加网卡成功后要将网卡设置为默认网卡。
	netif_set_default(&_lwip_netif);
}

void bsp::LwipEthernetInterface::InitializingNetifCallbackFunc()
{
#if LWIP_NETIF_HOSTNAME
	_lwip_netif.hostname = "lwip";
#endif

	_lwip_netif.name[0] = 'p';
	_lwip_netif.name[1] = 'n';

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	_lwip_netif.output = etharp_output;

	_lwip_netif.linkoutput = [](netif *net_interface, pbuf *p) -> err_t
	{
		try
		{
			bsp::LwipEthernetInterface::Instance().SendPbuf(p);
			return err_enum_t::ERR_OK;
		}
		catch (std::exception const &e)
		{
			return err_enum_t::ERR_IF;
		}
	};

	try
	{
		_ethernet_port->Open(_mac);
	}
	catch (std::exception const &e)
	{
		DI_Console().WriteLine(e.what());
		DI_Console().WriteLine("打开网口失败");
		netif_set_link_down(&_lwip_netif);
		netif_set_down(&_lwip_netif);
	}

	/* 设置MAC地址长度,为6个字节 */
	_lwip_netif.hwaddr_len = ETHARP_HWADDR_LEN;

	/* 初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复 */
	base::Span netif_mac_buff_span{_lwip_netif.hwaddr, 6};
	netif_mac_buff_span.CopyFrom(_mac.AsReadOnlySpan());
	netif_mac_buff_span.Reverse();

	_lwip_netif.mtu = ETH_MAX_PAYLOAD;

	/* 网卡状态信息标志位，是很重要的控制字段，它包括网卡功能使能、广播 */
	/* 使能、 ARP 使能等等重要控制位 */
	_lwip_netif.flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
}

void bsp::LwipEthernetInterface::SendPbuf(pbuf *p)
{
	pbuf *current_pbuf;
	base::List<base::ReadOnlySpan> spans{};

	for (current_pbuf = p; current_pbuf != nullptr; current_pbuf = current_pbuf->next)
	{
		base::ReadOnlySpan span{
			reinterpret_cast<uint8_t *>(current_pbuf->payload),
			current_pbuf->len,
		};

		spans.Add(span);
	}

	_ethernet_port->Send(spans);
}

bool bsp::LwipEthernetInterface::TryDHCP()
{
	/* 对IP地址、网关地址及子网页码清零操作 */
	_lwip_netif.ip_addr = ip_addr_t{};
	_lwip_netif.netmask = ip_addr_t{};
	_lwip_netif.gw = ip_addr_t{};

	DI_Console().WriteLine("开始进行 DHCP.");
	dhcp_start(&_lwip_netif);

	bool dhcp_result = false;
	for (int i = 0; i < 100; i++)
	{
		// 如果失败，最多重试 100 次。
		dhcp_result = dhcp_supplied_address(&_lwip_netif);
		if (dhcp_result)
		{
			break;
		}

		DI_Delayer().Delay(std::chrono::milliseconds{100});
	}

	if (!dhcp_result)
	{
		/* 使用静态IP地址 */
		IP4_ADDR(&(_lwip_netif.ip_addr),
				 _ip_address[0],
				 _ip_address[1],
				 _ip_address[2],
				 _ip_address[3]);

		IP4_ADDR(&(_lwip_netif.netmask),
				 _netmask[0],
				 _netmask[1],
				 _netmask[2],
				 _netmask[3]);

		IP4_ADDR(&(_lwip_netif.gw),
				 _gateway[0],
				 _gateway[1],
				 _gateway[2],
				 _gateway[3]);

		netif_set_addr(&_lwip_netif,
					   &_lwip_netif.ip_addr,
					   &_lwip_netif.netmask,
					   &_lwip_netif.gw);

		DI_Console().WriteLine("DHCP 超时。");

		// char ip_address_string_buffer[20] = {};

		// ip4addr_ntoa_r(netif_ip4_addr(&_lwip_netif),
		// 			   ip_address_string_buffer,
		// 			   sizeof(ip_address_string_buffer));

		// DI_Console().WriteLine(std::string{"静态 IP 地址："} + ip_address_string_buffer);

		return false;
	}

	DI_Console().WriteLine("DHCP 成功。");
	uint32_t ip = _lwip_netif.ip_addr.addr;      /* 读取新IP地址 */
	uint32_t netmask = _lwip_netif.netmask.addr; /* 读取子网掩码 */
	uint32_t gw = _lwip_netif.gw.addr;           /* 读取默认网关 */

	// 解析出通过DHCP获取到的IP地址
	_ip_address[3] = (uint8_t)(ip >> 24);
	_ip_address[2] = (uint8_t)(ip >> 16);
	_ip_address[1] = (uint8_t)(ip >> 8);
	_ip_address[0] = (uint8_t)(ip);

	DI_Console().WriteLine("通过 DHCP 获取到 IP 地址：" +
						   std::to_string(_ip_address[0]) + '.' +
						   std::to_string(_ip_address[1]) + '.' +
						   std::to_string(_ip_address[2]) + '.' +
						   std::to_string(_ip_address[3]));

	// 解析通过DHCP获取到的子网掩码地址
	_netmask[3] = (uint8_t)(netmask >> 24);
	_netmask[2] = (uint8_t)(netmask >> 16);
	_netmask[1] = (uint8_t)(netmask >> 8);
	_netmask[0] = (uint8_t)(netmask);

	DI_Console().WriteLine("通过DHCP获取到子网掩码：" +
						   std::to_string(_netmask[0]) + '.' +
						   std::to_string(_netmask[1]) + '.' +
						   std::to_string(_netmask[2]) + '.' +
						   std::to_string(_netmask[3]));

	// 解析出通过DHCP获取到的默认网关
	_gateway[3] = (uint8_t)(gw >> 24);
	_gateway[2] = (uint8_t)(gw >> 16);
	_gateway[1] = (uint8_t)(gw >> 8);
	_gateway[0] = (uint8_t)(gw);

	DI_Console().WriteLine("通过 DHCP 获取到的默认网关：" +
						   std::to_string(_gateway[0]) + '.' +
						   std::to_string(_gateway[1]) + '.' +
						   std::to_string(_gateway[2]) + '.' +
						   std::to_string(_gateway[3]));

	return true;
}

#pragma region 线程函数

void bsp::LwipEthernetInterface::InputThreadFunc()
{
	while (true)
	{
		base::IEnumerable<base::ReadOnlySpan> const &spans = _ethernet_port->Receive();
		pbuf *head_pbuf = nullptr;
		pbuf *current_pbuf = nullptr;

		for (base::ReadOnlySpan const &span : spans)
		{
			pbuf_custom *custom_pbuf = new pbuf_custom{};
			custom_pbuf->custom_free_function = [](pbuf *p)
			{
				delete reinterpret_cast<pbuf_custom *>(p);
			};

			pbuf *p = pbuf_alloced_custom(PBUF_RAW,
										  span.Size(),
										  PBUF_REF,
										  custom_pbuf,
										  const_cast<uint8_t *>(span.Buffer()),
										  span.Size());

			p->next = nullptr;
			if (head_pbuf == nullptr)
			{
				head_pbuf = p;
				current_pbuf = p;
			}
			else
			{
				current_pbuf->next = p;
				current_pbuf = p;
			}
		}

		// 串成链表后一次性输入。
		if (_lwip_netif.input(head_pbuf, &_lwip_netif) != err_enum_t::ERR_OK)
		{
			pbuf_free(head_pbuf);
		}
	}
}

void bsp::LwipEthernetInterface::LinkStateDetectingThreadFunc()
{
	while (true)
	{
		bool is_linked = _ethernet_port->IsLinked();
		if (is_linked == netif_is_up(&_lwip_netif))
		{
			DI_Delayer().Delay(std::chrono::milliseconds{100});
			continue;
		}

		if (is_linked)
		{
			// 开启以太网及虚拟网卡
			DI_Console().WriteLine("检测到网线插入");
			_ethernet_port->Restart();
			netif_set_up(&_lwip_netif);
			netif_set_link_up(&_lwip_netif);
			TryDHCP();
		}
		else
		{
#if LWIP_DHCP
			dhcp_stop(&_lwip_netif);
#endif

			/* LWIP_DHCP */
			DI_Console().WriteLine("检测到网线断开。");
			netif_set_down(&_lwip_netif);
			netif_set_link_down(&_lwip_netif);
		}
	}
}

#pragma endregion

bsp::LwipEthernetInterface &bsp::LwipEthernetInterface::Instance()
{
	class Getter :
		public base::SingletonGetter<LwipEthernetInterface>
	{
	public:
		std::unique_ptr<LwipEthernetInterface> Create() override
		{
			return std::unique_ptr<LwipEthernetInterface>{new LwipEthernetInterface{}};
		}

		void Lock() override
		{
			DI_DisableGlobalInterrupt();
		}

		void Unlock() override
		{
			DI_EnableGlobalInterrupt();
		}
	};

	Getter g;
	return g.Instance();
}

void bsp::LwipEthernetInterface::Open()
{
	tcpip_init(nullptr, nullptr);

#if !LWIP_DHCP
	_dhcpstatus = 0XFF;
#endif

	AddDefaultNetInterface();

	// 链接状态更新
	DI_TaskManager().Create(
		[this]()
		{
			LinkStateDetectingThreadFunc();
		},
		512);

	/* create the task that handles the ETH_MAC */
	DI_TaskManager().Create(
		[this]()
		{
			InputThreadFunc();
		},
		512);
}
