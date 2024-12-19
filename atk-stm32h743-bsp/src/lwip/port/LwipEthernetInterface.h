#pragma once
#include <base/define.h>
#include <base/net/IPAddress.h>
#include <base/net/Mac.h>
#include <bsp-interface/di/ethernet.h>
#include <lwip/err.h>
#include <lwip/netif.h>
#include <NetifWrapper.h>

namespace bsp
{
	/// @brief lwip 的网卡接口。
	class LwipEthernetInterface
	{
	private:
		LwipEthernetInterface() = default;

		bsp::IEthernetPort *_ethernet_port = &DI_EthernetPort();

		/// @brief 使用本网卡发送 pbuf 链表。
		/// @param p
		void SendPbuf(pbuf *p);

		bool TryDHCP();

#pragma region 线程函数
		/// @brief 负责将网口接收到的数据送给 lwip.
		void InputThreadFunc();

		/// @brief 检测链接状态的线程函数。
		void LinkStateDetectingThreadFunc();
#pragma endregion

		lwip::NetifWrapper _netif_wrapper;

		/// @brief 本机IP地址
		base::IPAddress _ip_address{
			std::endian::big,
			base::Array<uint8_t, 4>{192, 168, 1, 30},
		};

		base::IPAddress _netmask{
			std::endian::big,
			base::Array<uint8_t, 4>{255, 255, 255, 0},
		};

		base::IPAddress _gateway{
			std::endian::big,
			base::Array<uint8_t, 4>{192, 168, 1, 1},
		};

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
