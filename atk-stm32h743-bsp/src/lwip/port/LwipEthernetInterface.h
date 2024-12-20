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

#pragma region 线程函数
		/// @brief 负责将网口接收到的数据送给 lwip.
		void InputThreadFunc();

		/// @brief 检测链接状态的线程函数。
		void LinkStateDetectingThreadFunc();
#pragma endregion

		lwip::NetifWrapper _netif_wrapper;

	public:
		static_function LwipEthernetInterface &Instance();

		/// @brief 打开本网卡并作为 lwip 的默认网卡。
		void Open();
	};
} // namespace bsp
