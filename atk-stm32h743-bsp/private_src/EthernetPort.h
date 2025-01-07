#pragma once
#include <AtkApolloV2PhyController.h>
#include <base/define.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/ethernet/phy/YT8512CPhyDriver.h>

namespace bsp
{
	/// @brief 以太网端口。
	class EthernetPort :
		public bsp::IEthernetPort
	{
	private:
		bsp::IEthernetController *_controller = &DI_EthernetController();
		base::Delegate<base::ReadOnlySpan> _receiving_ethernet_frame_event;
		base::Delegate<> _connection_event;
		base::Delegate<> _disconnection_event;
		bsp::YT8512CPhyDriver _phy_driver{std::shared_ptr<bsp::IPhyController>{new AtkApolloV2PhyController{}}};

	public:
		static_function EthernetPort &Instance();

		/// @brief 以太网控制器的名称。
		/// @return
		virtual std::string Name() const override;

		/// @brief 打开以太网端口。
		/// @param mac MAC 地址。
		virtual void Open(base::Mac const &mac) override;

		/// @brief 发送。
		/// @param spans
		virtual void Send(base::IEnumerable<base::ReadOnlySpan> const &spans) override;

		/// @brief 发送单个 span.
		/// @param span
		virtual void Send(base::ReadOnlySpan const &span) override;

		/// @brief 收到以太网帧会触发此事件。
		/// @note 事件回调中会传入一个装有完整的以太网帧的 base::ReadOnlySpan.
		/// @return
		virtual base::IEvent<base::ReadOnlySpan> &ReceivingEhternetFrameEvent() override;

		/// @brief 连接事件。
		/// @note 链路层连接建立后会触发事件。
		/// @return
		virtual base::IEvent<> &ConnectionEvent() override;

		/// @brief 断开连接事件。
		/// @note 链路层连接断开后会触发事件。
		/// @return
		virtual base::IEvent<> &DisconnectionEvent() override;
	};
} // namespace bsp
