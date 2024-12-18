#pragma once
#include <base/define.h>
#include <lwip/err.h>
#include <lwip/netif.h>
#include <lwip_comm.h>

namespace bsp
{
	/// @brief lwip 的网卡接口。
	class LwipEthernetInterface
	{
	private:
		LwipEthernetInterface();

		/// @brief DHCP 线程函数。
		void DhcpThreadFunc();

		/// @brief 检测链接状态的线程函数。
		void LinkStateCheckingThreadFunc();

		/// @brief 初始化网卡的函数，被 netif_add 函数回调。
		void InitializingNetifCallbackFunc();

		void SendPbuf(pbuf *p);

		uint8_t _lwip_dhcp_state = LWIP_DHCP_OFF;

		/// @brief 网卡。
		netif _lwip_netif{};

		/// @brief 本机IP地址
		uint8_t _ip_address[4] = {};

		/// @brief 子网掩码
		uint8_t _netmask[4] = {};

		/// @brief 默认网关的IP地址
		uint8_t _gateway[4] = {};

		/// @brief DHCP 状态。
		///		0：未获取DHCP地址。
		/// 	1：进入DHCP获取状态。
		/// 	2：成功获取DHCP地址。
		/// 	0xff：获取失败。
		uint8_t _dhcpstatus = 0;

		/// @brief 连接状态。
		uint8_t link_status = 0;

	public:
		static_function LwipEthernetInterface &Instance();

		void Open();
	};
} // namespace bsp
