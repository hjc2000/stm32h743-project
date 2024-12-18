#pragma once
#include <base/define.h>
#include <base/net/IPAddress.h>
#include <base/net/Mac.h>
#include <bsp-interface/di/ethernet.h>
#include <lwip/err.h>
#include <lwip/netif.h>

namespace bsp
{
	/// @brief lwip 的网卡接口。
	class LwipEthernetInterface
	{
	private:
		LwipEthernetInterface();

		bsp::IEthernetPort *_ethernet_port = &DI_EthernetPort();

		/// @brief 向 lwip 添加默认网卡。
		void AddDefaultNetInterface();

		/// @brief 初始化网卡的函数，被 netif_add 函数回调。
		void InitializingNetifCallbackFunc();

		/// @brief 使用本网卡发送 pbuf 链表。
		/// @param p
		void SendPbuf(pbuf *p);

		bool TryDHCP();

#pragma region 线程函数
		/// @brief DHCP 线程函数。
		void DhcpThreadFunc();

		/// @brief 负责将网口接收到的数据送给 lwip.
		void InputThreadFunc();

		/// @brief 检测链接状态的线程函数。
		void LinkStateDetectingThreadFunc();
#pragma endregion

		uint8_t _lwip_dhcp_state = 0;

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

		/// @brief 本网卡所使用的 MAC 地址。
		base::Mac _mac{
			std::endian::big,
			base::Array<uint8_t, 6>{
				0xB8,
				0xAE,
				0x1D,
				0x00,
				0x04,
				0x00,
			},
		};

	public:
		static_function LwipEthernetInterface &Instance();

		/// @brief 打开本网卡并作为 lwip 的默认网卡。
		void Open();
	};
} // namespace bsp
