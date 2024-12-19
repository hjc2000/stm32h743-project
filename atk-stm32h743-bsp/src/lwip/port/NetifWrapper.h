#pragma once
#include <base/Convert.h>
#include <base/net/IPAddress.h>
#include <base/net/Mac.h>
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
		NetifWrapper();

		void Open(base::IPAddress const &ip_address,
				  base::IPAddress const &netmask,
				  base::IPAddress const &gateway);

		/// @brief 获取被包装对象的指针。
		/// @return
		netif *WrappedObj() const override;

		base::IPAddress IPAddress() const;
		void SetIPAddress(base::IPAddress const &value);

		base::IPAddress Netmask() const;
		void SetNetmask(base::IPAddress const &value);

		base::IPAddress Gateway() const;
		void SetGateway(base::IPAddress const &value);

		/// @brief 直接将全部地址改了。
		/// @note 如果实际需求是修改多个地址，最好使用本方法，因为修改地址后要做一次配置，
		/// 让 lwip 使用新的地址。修改一个地址是配置一次，修改多个地址也是配置一次。所以
		/// 如果是修改多个地址，使用本方法会比对每个地址单独调用 Set 方法的性能好。
		/// @param ip_address
		/// @param netmask
		/// @param gateway
		void SetAllAddress(base::IPAddress const &ip_address,
						   base::IPAddress const &netmask,
						   base::IPAddress const &gateway);

		/// @brief 启动 DHCP.
		void StartDHCP();

		/// @brief 停止 DHCP.
		void StopDHCP();

		/// @brief 设置为默认网卡。
		void SetAsDefaultNetInterface();
	};
} // namespace lwip
