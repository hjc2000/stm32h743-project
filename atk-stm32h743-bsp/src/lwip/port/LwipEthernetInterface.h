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

		void Open();
	};
} // namespace bsp
