#pragma once
#include <AtkApolloV2PhyController.h>
#include <base/define.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/ethernet/phy/YU8512CPhyDriver.h>

namespace bsp
{
	/// @brief 以太网端口。
	class EthernetPort :
		public bsp::IEthernetPort
	{
	private:
		bsp::IEthernetController *_controller = &DI_EthernetController();
		base::Delegate<base::ReadOnlySpan> _receiving_ethernet_frame_event;
		bsp::YU8512CPhyDriver _phy_driver{std::shared_ptr<bsp::IPhyController>{new AtkApolloV2PhyController{}}};

	public:
		static_function EthernetPort &Instance();

		/// @brief 以太网控制器的名称。
		/// @return
		std::string Name() const override;

		/// @brief 打开以太网端口。
		/// @param mac MAC 地址。
		void Open(base::Mac const &mac) override;

		/// @brief 重启网口。
		/// @note 会保留 MAC 地址等配置。
		/// @note 会重新进行自动协商的过程。断线重连后可以调用本方法，防止 MAC 控制器
		/// 所使用的速率、双工等配置与新插入的网线不符。
		void Restart() override;

		/// @brief 发送。
		/// @param spans
		void Send(base::IEnumerable<base::ReadOnlySpan> const &spans) override;

		/// @brief 发送单个 span.
		/// @param span
		void Send(base::ReadOnlySpan const &span) override;

		/// @brief 接收。
		/// @note 因为接收后需要解析，而解析需要数据完整且连续，所以必须接收一整个完整的以太网帧，
		/// 放到一个 span 中。
		/// @return
		base::ReadOnlySpan Receive();

		/// @brief 收到以太网帧会触发此事件。
		/// @note 事件回调中会传入一个装有完整的以太网帧的 base::ReadOnlySpan.
		/// @return
		base::IEvent<base::ReadOnlySpan> &ReceivintEhternetFrameEvent() override;

		/// @brief 网线当前处于链接状态。
		/// @return
		virtual bool IsLinked() override;
	};
} // namespace bsp
