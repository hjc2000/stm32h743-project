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

uint32_t g_dhcp_fine_timer = 0;                 /* DHCP精细处理计时器 */
__IO uint8_t g_lwip_dhcp_state = LWIP_DHCP_OFF; /* DHCP状态初始化 */

/**
 * @brief       通知用户DHCP配置状态
 * @param       netif：网卡控制块
 * @retval      无
 */
void lwip_link_status_updated(netif *netif)
{
	if (netif_is_up(netif))
	{
#if LWIP_DHCP
		/* Update DHCP state machine */
		g_lwip_dhcp_state = LWIP_DHCP_START;
#endif
		/* LWIP_DHCP */
		printf("The network cable is connected \r\n");
	}
	else
	{
#if LWIP_DHCP
		/* Update DHCP state machine */
		g_lwip_dhcp_state = LWIP_DHCP_LINK_DOWN;
#endif
		/* LWIP_DHCP */
		printf("The network cable is not connected \r\n");
	}
}

/**
 * @brief       检查ETH链路状态，更新netif
 * @param       argument: netif
 * @retval      无
 */
void lwip_link_thread()
{
	DI_Console().WriteLine("enter lwip_link_thread");

	int link_again_num = 0;

	while (true)
	{
		/* 读取PHY状态寄存器，获取链接信息 */
		uint32_t regval = DI_EthernetController().ReadPHYRegister(1);

		/* 判断链接状态 */
		if ((regval & 0x0004U) == 0)
		{
			bsp::LwipEthernetInterface::Instance().link_status = LWIP_LINK_OFF;
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
				g_lwip_dhcp_state = LWIP_DHCP_LINK_DOWN;
				dhcp_stop(&bsp::LwipEthernetInterface::Instance()._lwip_netif);
#endif
				netif_set_down(&bsp::LwipEthernetInterface::Instance()._lwip_netif);
				netif_set_link_down(&bsp::LwipEthernetInterface::Instance()._lwip_netif);
			}
		}
		else
		{
			/* 网线插入检测 */
			link_again_num = 0;
			if (bsp::LwipEthernetInterface::Instance().link_status == LWIP_LINK_OFF)
			{
				/* 开启以太网及虚拟网卡 */
				DI_EthernetPort().Restart();

				bsp::LwipEthernetInterface::Instance().link_status = LWIP_LINK_ON;
				netif_set_up(&bsp::LwipEthernetInterface::Instance()._lwip_netif);
				netif_set_link_up(&bsp::LwipEthernetInterface::Instance()._lwip_netif);
			}
		}

		DI_Delayer().Delay(std::chrono::milliseconds{100});
	}
}

/**
 * @breif       DHCP进程
 * @param       argument:传入的形参
 * @retval      无
 */
void lwip_periodic_handle()
{
#if LWIP_DHCP
	DI_Console().WriteLine("enter lwip_periodic_handle");

	uint32_t ip = 0;
	uint32_t netmask = 0;
	uint32_t gw = 0;
	struct dhcp *dhcp;
	uint8_t iptxt[20];

	while (true)
	{
		switch (g_lwip_dhcp_state)
		{
		case LWIP_DHCP_START:
			{
				/* 对IP地址、网关地址及子网页码清零操作 */
				ip_addr_set_zero_ip4(&bsp::LwipEthernetInterface::Instance()._lwip_netif.ip_addr);
				ip_addr_set_zero_ip4(&bsp::LwipEthernetInterface::Instance()._lwip_netif.netmask);
				ip_addr_set_zero_ip4(&bsp::LwipEthernetInterface::Instance()._lwip_netif.gw);
				ip_addr_set_zero_ip4(&bsp::LwipEthernetInterface::Instance()._lwip_netif.ip_addr);
				ip_addr_set_zero_ip4(&bsp::LwipEthernetInterface::Instance()._lwip_netif.netmask);
				ip_addr_set_zero_ip4(&bsp::LwipEthernetInterface::Instance()._lwip_netif.gw);

				g_lwip_dhcp_state = LWIP_DHCP_WAIT_ADDRESS;

				printf("State: Looking for DHCP server ...\r\n");
				dhcp_start(&bsp::LwipEthernetInterface::Instance()._lwip_netif);
				break;
			}
		case LWIP_DHCP_WAIT_ADDRESS:
			{
				if (dhcp_supplied_address(&bsp::LwipEthernetInterface::Instance()._lwip_netif))
				{
					g_lwip_dhcp_state = LWIP_DHCP_ADDRESS_ASSIGNED;

					ip = bsp::LwipEthernetInterface::Instance()._lwip_netif.ip_addr.addr;      /* 读取新IP地址 */
					netmask = bsp::LwipEthernetInterface::Instance()._lwip_netif.netmask.addr; /* 读取子网掩码 */
					gw = bsp::LwipEthernetInterface::Instance()._lwip_netif.gw.addr;           /* 读取默认网关 */

					sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(&bsp::LwipEthernetInterface::Instance()._lwip_netif)));
					printf("IP address assigned by a DHCP server: %s\r\n", iptxt);

					if (ip != 0)
					{
						bsp::LwipEthernetInterface::Instance()._dhcpstatus = 2; /* DHCP成功 */
						/* 解析出通过DHCP获取到的IP地址 */
						bsp::LwipEthernetInterface::Instance().ip[3] = (uint8_t)(ip >> 24);
						bsp::LwipEthernetInterface::Instance().ip[2] = (uint8_t)(ip >> 16);
						bsp::LwipEthernetInterface::Instance().ip[1] = (uint8_t)(ip >> 8);
						bsp::LwipEthernetInterface::Instance().ip[0] = (uint8_t)(ip);
						printf("通过DHCP获取到IP地址..............%d.%d.%d.%d\r\n", bsp::LwipEthernetInterface::Instance().ip[0], bsp::LwipEthernetInterface::Instance().ip[1], bsp::LwipEthernetInterface::Instance().ip[2], bsp::LwipEthernetInterface::Instance().ip[3]);
						/* 解析通过DHCP获取到的子网掩码地址 */
						bsp::LwipEthernetInterface::Instance().netmask[3] = (uint8_t)(netmask >> 24);
						bsp::LwipEthernetInterface::Instance().netmask[2] = (uint8_t)(netmask >> 16);
						bsp::LwipEthernetInterface::Instance().netmask[1] = (uint8_t)(netmask >> 8);
						bsp::LwipEthernetInterface::Instance().netmask[0] = (uint8_t)(netmask);
						printf("通过DHCP获取到子网掩码............%d.%d.%d.%d\r\n", bsp::LwipEthernetInterface::Instance().netmask[0], bsp::LwipEthernetInterface::Instance().netmask[1], bsp::LwipEthernetInterface::Instance().netmask[2], bsp::LwipEthernetInterface::Instance().netmask[3]);
						/* 解析出通过DHCP获取到的默认网关 */
						bsp::LwipEthernetInterface::Instance()._gateway[3] = (uint8_t)(gw >> 24);
						bsp::LwipEthernetInterface::Instance()._gateway[2] = (uint8_t)(gw >> 16);
						bsp::LwipEthernetInterface::Instance()._gateway[1] = (uint8_t)(gw >> 8);
						bsp::LwipEthernetInterface::Instance()._gateway[0] = (uint8_t)(gw);

						printf("通过DHCP获取到的默认网关..........%d.%d.%d.%d\r\n",
							   bsp::LwipEthernetInterface::Instance()._gateway[0],
							   bsp::LwipEthernetInterface::Instance()._gateway[1],
							   bsp::LwipEthernetInterface::Instance()._gateway[2],
							   bsp::LwipEthernetInterface::Instance()._gateway[3]);
					}
				}
				else
				{
					dhcp = (struct dhcp *)netif_get_client_data(&bsp::LwipEthernetInterface::Instance()._lwip_netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

					/* DHCP timeout */
					if (dhcp->tries > LWIP_MAX_DHCP_TRIES)
					{
						g_lwip_dhcp_state = LWIP_DHCP_TIMEOUT;
						bsp::LwipEthernetInterface::Instance()._dhcpstatus = 0XFF;
						/* 使用静态IP地址 */
						IP4_ADDR(&(bsp::LwipEthernetInterface::Instance()._lwip_netif.ip_addr), bsp::LwipEthernetInterface::Instance().ip[0], bsp::LwipEthernetInterface::Instance().ip[1], bsp::LwipEthernetInterface::Instance().ip[2], bsp::LwipEthernetInterface::Instance().ip[3]);
						IP4_ADDR(&(bsp::LwipEthernetInterface::Instance()._lwip_netif.netmask), bsp::LwipEthernetInterface::Instance().netmask[0], bsp::LwipEthernetInterface::Instance().netmask[1], bsp::LwipEthernetInterface::Instance().netmask[2], bsp::LwipEthernetInterface::Instance().netmask[3]);
						IP4_ADDR(&(bsp::LwipEthernetInterface::Instance()._lwip_netif.gw), bsp::LwipEthernetInterface::Instance()._gateway[0], bsp::LwipEthernetInterface::Instance()._gateway[1], bsp::LwipEthernetInterface::Instance()._gateway[2], bsp::LwipEthernetInterface::Instance()._gateway[3]);
						netif_set_addr(&bsp::LwipEthernetInterface::Instance()._lwip_netif, &bsp::LwipEthernetInterface::Instance()._lwip_netif.ip_addr, &bsp::LwipEthernetInterface::Instance()._lwip_netif.netmask, &bsp::LwipEthernetInterface::Instance()._lwip_netif.gw);

						sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(&bsp::LwipEthernetInterface::Instance()._lwip_netif)));
						printf("DHCP Timeout !! \r\n");
						printf("Static IP address: %s\r\n", iptxt);
					}
				}

				break;
			}
		case LWIP_DHCP_LINK_DOWN:
			{
				g_lwip_dhcp_state = LWIP_DHCP_OFF;
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

bsp::LwipEthernetInterface::LwipEthernetInterface()
{
	/* 默认远端IP为:192.168.1.134 */
	_remoteip[0] = 192;
	_remoteip[1] = 168;
	_remoteip[2] = 1;
	_remoteip[3] = 134;

	/* 默认本地IP为:192.168.1.30 */
	ip[0] = 192;
	ip[1] = 168;
	ip[2] = 1;
	ip[3] = 30;

	/* 默认子网掩码:255.255.255.0 */
	netmask[0] = 255;
	netmask[1] = 255;
	netmask[2] = 255;
	netmask[3] = 0;

	/* 默认网关:192.168.1.1 */
	_gateway[0] = 192;
	_gateway[1] = 168;
	_gateway[2] = 1;
	_gateway[3] = 1;

	_dhcpstatus = 0; /* 没有DHCP */
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
										(ip_addr_t const *)&ip,
										(ip_addr_t const *)&netmask,
										(ip_addr_t const *)&_gateway,
										nullptr,
										&ethernetif_init,
										&tcpip_input);

	if (netif_add_result == nullptr)
	{
		DI_Console().WriteLine("添加网卡失败。");
		throw std::runtime_error{"添加网卡失败。"};
	}

	// 添加网卡成功后将其设置为默认网卡
	netif_set_default(&_lwip_netif);

#if LWIP_NETIF_LINK_CALLBACK
	/* DHCP链接状态更新函数 */
	lwip_link_status_updated(&_lwip_netif);
	netif_set_link_callback(&_lwip_netif, lwip_link_status_updated);

	DI_TaskManager().Create(
		[]()
		{
			lwip_link_thread();
		},
		512);
#endif

	link_status = LWIP_LINK_OFF; /* 链接标记为0 */

#if LWIP_DHCP        /* 如果使用DHCP的话 */
	_dhcpstatus = 0; /* DHCP标记为0 */

	DI_TaskManager().Create(
		[]()
		{
			lwip_periodic_handle();
		},
		512);
#endif
}
