#pragma once
#include <base/define.h>
#include <lwip/err.h>
#include <lwip/netif.h>

namespace bsp
{
	/// @brief lwip 的网卡接口。
	class LwipEthernetInterface
	{
	private:
		LwipEthernetInterface();

	public:
		static_function LwipEthernetInterface &Instance();

		netif _lwip_netif{};

		uint8_t remoteip[4]; /* 远端主机IP地址 */
		uint8_t ip[4];       /* 本机IP地址 */
		uint8_t netmask[4];  /* 子网掩码 */
		uint8_t gateway[4];  /* 默认网关的IP地址 */

		uint8_t _dhcpstatus; /* dhcp状态
							0, 未获取DHCP地址
							1, 进入DHCP获取状态
							2, 成功获取DHCP地址
							0XFF,获取失败 */

		uint8_t link_status; /* 连接状态 */

		void Open();
	};
} // namespace bsp
