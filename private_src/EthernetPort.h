#pragma once
#include "base/delegate/Delegate.h"
#include "base/embedded/ethernet/EthernetController.h"
#include "base/embedded/ethernet/IEthernetPort.h"
#include "base/embedded/ethernet/phy/YT8512CPhyDriver.h"
#include "base/task/Mutex.h"

namespace bsp
{
	///
	/// @brief 以太网端口。
	///
	class EthernetPort :
		public base::ethernet::IEthernetPort
	{
	private:
		base::ethernet::EthernetController _controller{1};
		base::task::Mutex _sending_lock{};
		base::Delegate<base::ReadOnlySpan> _receiving_ethernet_frame_event;
		base::Delegate<> _connected_event;
		base::Delegate<> _disconnected_event;
		base::ethernet::YT8512CPhyDriver _phy_driver{_controller};

	public:
		///
		/// @brief 打开以太网端口。
		///
		/// @param mac MAC 地址。
		///
		virtual void Open(base::Mac const &mac) override;

		///
		/// @brief 发送。
		///
		/// @param spans
		///
		virtual void Send(std::vector<base::ReadOnlySpan> const &spans) override;

		///
		/// @brief 发送单个 span.
		///
		/// @param span
		///
		virtual void Send(base::ReadOnlySpan const &span) override;

		///
		/// @brief 收到以太网帧会触发此事件。
		///
		/// @note 事件回调中会传入一个装有完整的以太网帧的 base::ReadOnlySpan.
		///
		/// @return
		///
		virtual base::IEvent<base::ReadOnlySpan> &ReceivingEhternetFrameEvent() override;

		///
		/// @brief 连接事件。
		///
		/// @note 链路层连接建立后会触发事件。
		///
		/// @return
		///
		virtual base::IEvent<> &ConnectedEvent() override;

		///
		/// @brief 断开连接事件。
		///
		/// @note 链路层连接断开后会触发事件。
		///
		/// @return
		///
		virtual base::IEvent<> &DisconnectedEvent() override;
	};
} // namespace bsp
