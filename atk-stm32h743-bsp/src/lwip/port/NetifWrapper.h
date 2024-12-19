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
	/// @warning 本类依赖 netif 的 state 字段。使用本类后，禁止修改此字段。
	class NetifWrapper :
		public base::Wrapper<netif>
	{
	private:
		std::unique_ptr<netif> _wrapped_obj{new netif{}};

#pragma region 初始化回调

		class
		{
		public:
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

			int32_t _mtu = 1500;

		} _init_callback_func_context;

		void InitializationCallbackFunc();
#pragma endregion

	public:
		NetifWrapper();

		void Open(base::Mac const &mac,
				  base::IPAddress const &ip_address,
				  base::IPAddress const &netmask,
				  base::IPAddress const &gateway,
				  int32_t mtu);

		/// @brief 获取被包装对象的指针。
		/// @return
		netif *WrappedObj() const override;

#pragma region 地址
		base::Mac Mac() const;
		void SetMac(base::Mac const &o);

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

		/// @brief 将所有地址清 0.
		/// @note 包括：IP 地址、子网掩码、网关。
		void ClearAllAddress();
#pragma endregion

		/// @brief 启动 DHCP.
		void StartDHCP();

		/// @brief 停止 DHCP.
		void StopDHCP();

		/// @brief 检查本次启动 DHCP 后 IP 地址是否被 DHCP 提供了。
		/// @return 如果 DHCP 提供了 IP 地址，则返回 true, 否则返回 false.
		bool DhcpSuppliedAddress();

		/// @brief 设置为默认网卡。
		void SetAsDefaultNetInterface();
	};
} // namespace lwip
