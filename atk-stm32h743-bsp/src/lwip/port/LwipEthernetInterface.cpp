#include "LwipEthernetInterface.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <ethernetif.h>
#include <FreeRTOS.h>
#include <hal.h>
#include <lwip/dhcp.h>
#include <lwip/init.h>
#include <lwip/mem.h>
#include <lwip/memp.h>
#include <lwip/tcpip.h>
#include <lwip/timeouts.h>
#include <lwip_comm.h>
#include <netif/etharp.h>
#include <stdint.h>
#include <stdio.h>
#include <task.h>

bsp::LwipEthernetInterface::LwipEthernetInterface()
{
	/* 默认远端IP为:192.168.1.134 */
	_remoteip[0] = 192;
	_remoteip[1] = 168;
	_remoteip[2] = 1;
	_remoteip[3] = 134;

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

	_dhcpstatus = 0; /* 没有DHCP */
}

void bsp::LwipEthernetInterface::DhcpThreadFunc()
{
#if LWIP_DHCP
	uint32_t ip = 0;
	uint32_t netmask = 0;
	uint32_t gw = 0;
	struct dhcp *dhcp;
	uint8_t iptxt[20];

	while (true)
	{
		switch (_lwip_dhcp_state)
		{
		case LWIP_DHCP_START:
			{
				/* 对IP地址、网关地址及子网页码清零操作 */
				ip_addr_set_zero_ip4(&_lwip_netif.ip_addr);
				ip_addr_set_zero_ip4(&_lwip_netif.netmask);
				ip_addr_set_zero_ip4(&_lwip_netif.gw);
				ip_addr_set_zero_ip4(&_lwip_netif.ip_addr);
				ip_addr_set_zero_ip4(&_lwip_netif.netmask);
				ip_addr_set_zero_ip4(&_lwip_netif.gw);

				_lwip_dhcp_state = LWIP_DHCP_WAIT_ADDRESS;

				printf("State: Looking for DHCP server ...\r\n");
				dhcp_start(&_lwip_netif);
				break;
			}
		case LWIP_DHCP_WAIT_ADDRESS:
			{
				if (dhcp_supplied_address(&_lwip_netif))
				{
					_lwip_dhcp_state = LWIP_DHCP_ADDRESS_ASSIGNED;

					ip = _lwip_netif.ip_addr.addr;      /* 读取新IP地址 */
					netmask = _lwip_netif.netmask.addr; /* 读取子网掩码 */
					gw = _lwip_netif.gw.addr;           /* 读取默认网关 */

					// sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(&_lwip_netif)));
					// printf("IP address assigned by a DHCP server: %s\r\n", iptxt);

					if (ip != 0)
					{
						_dhcpstatus = 2; /* DHCP成功 */
						/* 解析出通过DHCP获取到的IP地址 */
						_ip_address[3] = (uint8_t)(ip >> 24);
						_ip_address[2] = (uint8_t)(ip >> 16);
						_ip_address[1] = (uint8_t)(ip >> 8);
						_ip_address[0] = (uint8_t)(ip);

						DI_Console().WriteLine("通过 DHCP 获取到 IP 地址：" +
											   std::to_string(_ip_address[0]) + '.' +
											   std::to_string(_ip_address[1]) + '.' +
											   std::to_string(_ip_address[2]) + '.' +
											   std::to_string(_ip_address[3]));

						/* 解析通过DHCP获取到的子网掩码地址 */
						_netmask[3] = (uint8_t)(netmask >> 24);
						_netmask[2] = (uint8_t)(netmask >> 16);
						_netmask[1] = (uint8_t)(netmask >> 8);
						_netmask[0] = (uint8_t)(netmask);

						DI_Console().WriteLine("通过DHCP获取到子网掩码：" +
											   std::to_string(_netmask[0]) + '.' +
											   std::to_string(_netmask[1]) + '.' +
											   std::to_string(_netmask[2]) + '.' +
											   std::to_string(_netmask[3]));

						/* 解析出通过DHCP获取到的默认网关 */
						_gateway[3] = (uint8_t)(gw >> 24);
						_gateway[2] = (uint8_t)(gw >> 16);
						_gateway[1] = (uint8_t)(gw >> 8);
						_gateway[0] = (uint8_t)(gw);

						DI_Console().WriteLine("通过 DHCP 获取到的默认网关：" +
											   std::to_string(_gateway[0]) + '.' +
											   std::to_string(_gateway[1]) + '.' +
											   std::to_string(_gateway[2]) + '.' +
											   std::to_string(_gateway[3]));
					}
				}
				else
				{
					dhcp = (struct dhcp *)netif_get_client_data(&_lwip_netif,
																LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

					/* DHCP timeout */
					if (dhcp->tries > LWIP_MAX_DHCP_TRIES)
					{
						_lwip_dhcp_state = LWIP_DHCP_TIMEOUT;
						_dhcpstatus = 0XFF;

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

						sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(&_lwip_netif)));
						printf("DHCP Timeout !! \r\n");
						printf("Static IP address: %s\r\n", iptxt);
					}
				}

				break;
			}
		case LWIP_DHCP_LINK_DOWN:
			{
				_lwip_dhcp_state = LWIP_DHCP_OFF;
				break;
			}
		default:
			{
				break;
			}
		}

		DI_Delayer().Delay(std::chrono::milliseconds{1000});
	}

#endif
}

void bsp::LwipEthernetInterface::LinkStateCheckingThreadFunc()
{
	int link_again_num = 0;
	while (true)
	{
		/* 读取PHY状态寄存器，获取链接信息 */
		uint32_t regval = DI_EthernetController().ReadPHYRegister(1);

		/* 判断链接状态 */
		if ((regval & 0x0004U) == 0)
		{
			link_status = LWIP_LINK_OFF;
			link_again_num++;

			if (link_again_num >= 2)
			{
				/* 网线一段时间没有插入 */
				continue;
			}
			else
			{
				/* 关闭虚拟网卡及以太网中断 */
				DI_Console().WriteLine("close ethernet");

#if LWIP_DHCP
				_lwip_dhcp_state = LWIP_DHCP_LINK_DOWN;
				dhcp_stop(&_lwip_netif);
#endif
				netif_set_down(&_lwip_netif);
				netif_set_link_down(&_lwip_netif);
			}
		}
		else
		{
			/* 网线插入检测 */
			link_again_num = 0;
			if (link_status == LWIP_LINK_OFF)
			{
				/* 开启以太网及虚拟网卡 */
				DI_EthernetPort().Restart();

				link_status = LWIP_LINK_ON;
				netif_set_up(&_lwip_netif);
				netif_set_link_up(&_lwip_netif);
			}
		}

		DI_Delayer().Delay(std::chrono::milliseconds{100});
	}
}

void bsp::LwipEthernetInterface::UpdataLinkState()
{
	if (netif_is_up(&_lwip_netif))
	{
#if LWIP_DHCP
		/* Update DHCP state machine */
		_lwip_dhcp_state = LWIP_DHCP_START;
#endif
		/* LWIP_DHCP */
		printf("The network cable is connected \r\n");
	}
	else
	{
#if LWIP_DHCP
		/* Update DHCP state machine */
		_lwip_dhcp_state = LWIP_DHCP_LINK_DOWN;
#endif
		/* LWIP_DHCP */
		printf("The network cable is not connected \r\n");
	}
}

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
	tcpip_init(NULL, NULL);

#if !LWIP_DHCP
	_dhcpstatus = 0XFF;
#endif

	/* 向网卡列表中添加一个网口 */
	netif *netif_add_result = netif_add(&_lwip_netif,
										reinterpret_cast<ip_addr_t const *>(&_ip_address),
										reinterpret_cast<ip_addr_t const *>(&_netmask),
										reinterpret_cast<ip_addr_t const *>(&_gateway),
										nullptr,
										ethernetif_init,
										tcpip_input);

	if (netif_add_result == nullptr)
	{
		DI_Console().WriteLine("添加网卡失败。");
		throw std::runtime_error{"添加网卡失败。"};
	}

	// 添加网卡成功后将其设置为默认网卡
	netif_set_default(&_lwip_netif);

#if LWIP_NETIF_LINK_CALLBACK
	/* DHCP链接状态更新函数 */
	UpdataLinkState();
	netif_set_link_callback(&_lwip_netif,
							[](netif *p)
							{
								bsp::LwipEthernetInterface::Instance().UpdataLinkState();
							});

	DI_TaskManager().Create(
		[this]()
		{
			LinkStateCheckingThreadFunc();
		},
		512);
#endif

	link_status = LWIP_LINK_OFF; /* 链接标记为0 */

#if LWIP_DHCP        /* 如果使用DHCP的话 */
	_dhcpstatus = 0; /* DHCP标记为0 */

	DI_TaskManager().Create(
		[this]()
		{
			DhcpThreadFunc();
		},
		512);
#endif

	// 等待静态和动态分配完成
	while (_dhcpstatus != 2 && _dhcpstatus != 0xff)
	{
		DI_Delayer().Delay(std::chrono::milliseconds{500});
	}
}
