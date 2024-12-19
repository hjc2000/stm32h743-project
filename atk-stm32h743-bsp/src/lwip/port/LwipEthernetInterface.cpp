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
#include <lwip_convert.h>
#include <netif/etharp.h>
#include <stdint.h>
#include <stdio.h>
#include <task.h>

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
	_netif_wrapper->ip_addr = ip_addr_t{};
	_netif_wrapper->netmask = ip_addr_t{};
	_netif_wrapper->gw = ip_addr_t{};

	DI_Console().WriteLine("开始进行 DHCP.");
	_netif_wrapper.StartDHCP();

	bool dhcp_result = false;
	for (int i = 0; i < 50; i++)
	{
		// 如果失败，最多重试 100 次。
		dhcp_result = dhcp_supplied_address(_netif_wrapper);
		if (dhcp_result)
		{
			break;
		}

		DI_Delayer().Delay(std::chrono::milliseconds{100});
	}

	if (!dhcp_result)
	{
		/* 使用静态IP地址 */
		_netif_wrapper.SetIPAddress(_ip_address);
		_netif_wrapper.SetNetmask(_netmask);
		_netif_wrapper.SetGateway(_gateway);
		DI_Console().WriteLine("DHCP 超时。使用静态 IP 地址：" + _netif_wrapper.IPAddress().ToString());
		return false;
	}

	DI_Console().WriteLine("DHCP 成功。");

	_ip_address = _netif_wrapper.IPAddress();
	_netmask = _netif_wrapper.Netmask();
	_gateway = _netif_wrapper.Gateway();
	DI_Console().WriteLine("通过 DHCP 获取到 IP 地址：" + _ip_address.ToString());
	DI_Console().WriteLine("通过DHCP获取到子网掩码：" + _netmask.ToString());
	DI_Console().WriteLine("通过 DHCP 获取到的默认网关：" + _gateway.ToString());
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
		if (_netif_wrapper->input(head_pbuf, _netif_wrapper) != err_enum_t::ERR_OK)
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
		if (is_linked == netif_is_up(_netif_wrapper))
		{
			DI_Delayer().Delay(std::chrono::milliseconds{100});
			continue;
		}

		if (is_linked)
		{
			// 开启以太网及虚拟网卡
			DI_Console().WriteLine("检测到网线插入");
			_ethernet_port->Restart();
			netif_set_up(_netif_wrapper);
			netif_set_link_up(_netif_wrapper);
			TryDHCP();
		}
		else
		{
			_netif_wrapper.StopDHCP();

			/* LWIP_DHCP */
			DI_Console().WriteLine("检测到网线断开。");
			netif_set_down(_netif_wrapper);
			netif_set_link_down(_netif_wrapper);
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
	_ethernet_port->Open(_mac);
	tcpip_init(nullptr, nullptr);
	_netif_wrapper.Open(_ip_address, _netmask, _gateway);
	_netif_wrapper.SetAsDefaultNetInterface();

	/* 设置MAC地址长度,为6个字节 */
	_netif_wrapper->hwaddr_len = ETHARP_HWADDR_LEN;

	/* 初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复 */
	base::Span netif_mac_buff_span{_netif_wrapper->hwaddr, 6};
	netif_mac_buff_span.CopyFrom(_mac.AsReadOnlySpan());
	netif_mac_buff_span.Reverse();

	_netif_wrapper->mtu = ETH_MAX_PAYLOAD;

	/* 网卡状态信息标志位，是很重要的控制字段，它包括网卡功能使能、广播 */
	/* 使能、 ARP 使能等等重要控制位 */
	_netif_wrapper->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	_netif_wrapper->output = etharp_output;

	_netif_wrapper->linkoutput = [](netif *net_interface, pbuf *p) -> err_t
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
