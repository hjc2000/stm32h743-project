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
		void SetIPAddress(base::IPAddress const &ip_address);

		void StartDHCP();
		void StopDHCP();

		void SetAsDefaultNetInterface();
	};
} // namespace lwip
