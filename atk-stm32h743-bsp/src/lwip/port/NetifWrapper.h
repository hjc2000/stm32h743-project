#pragma once
#include <base/net/IPAddress.h>
#include <base/Wrapper.h>
#include <lwip/netif.h>
#include <memory>

namespace lwip
{
	/// @brief lwip 的 netif 的包装器。
	class NetifWrapper :
		public base::Wrapper<netif>
	{
	private:
		std::unique_ptr<netif> _wrapped_obj{new netif{}};

	public:
		/// @brief 获取被包装对象的指针。
		/// @return
		netif *WrappedObj() const override;

		base::IPAddress IPAddress() const;
		void SetIPAddress(base::IPAddress const &value);

		base::IPAddress Netmask() const;
		void SetNetmask(base::IPAddress const &value);

		/// @brief 启动 DHCP.
		void StartDHCP();

		/// @brief 停止 DHCP.
		void StopDHCP();

		/// @brief 设置为默认网卡。
		void SetAsDefaultNetInterface();
	};
} // namespace lwip
