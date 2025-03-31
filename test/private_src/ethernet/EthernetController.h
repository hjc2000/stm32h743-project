#pragma once
#include "base/container/List.h"
#include "base/define.h"
#include "base/task/IBinarySemaphore.h"
#include "bsp-interface/di/task.h"
#include "bsp-interface/ethernet/IEthernetController.h"
#include "hal.h"

namespace bsp
{
	class EthernetController :
		public bsp::IEthernetController
	{
	private:
		EthernetController();

		ETH_HandleTypeDef _handle{};
		bsp::EthernetInterfaceType _interface_type;
		uint32_t _phy_address = 0;
		base::Mac _mac;
		ETH_TxPacketConfig _sending_config{};
		base::List<ETH_BufferTypeDef> _eth_buffers{};
		std::shared_ptr<base::IBinarySemaphore> _send_completion_signal = base::CreateIBinarySemaphore(false);
		base::List<base::ReadOnlySpan> _received_span_list{};
		std::shared_ptr<base::IBinarySemaphore> _receiving_completion_signal = base::CreateIBinarySemaphore(false);

		/// @brief 接收。
		/// @return
		base::IEnumerable<base::ReadOnlySpan> const &ReceiveMultiSpans();

	public:
		static_function EthernetController &Instance();

		/// @brief 以太网控制器的名称。
		/// @return
		std::string Name() const override;

		/// @brief 获取本控制器绑定的 MAC 地址。
		/// @note 要打开本控制器后本属性才有效。
		/// @return
		base::Mac Mac() const override;

		/// @brief 打开以太网控制器。
		/// @param interface_type 连接着 PHY 的接口类型。（使用的是 MII 还是 RMII）
		/// @param phy_address PHY 的地址。
		/// @param mac MAC 地址。
		void Open(bsp::EthernetInterfaceType interface_type,
				  uint32_t phy_address,
				  base::Mac const &mac) override;

		/// @brief 读 PHY 的寄存器
		/// @param register_index 寄存器索引。
		/// @return
		uint32_t ReadPHYRegister(uint32_t register_index) override;

		/// @brief 写 PHY 的寄存器。
		/// @param register_index 寄存器索引。
		/// @param value
		void WritePHYRegister(uint32_t register_index, uint32_t value) override;

		/// @brief 启动以太网。
		/// @param duplex_mode
		/// @param speed
		void Start(bsp::EthernetDuplexMode duplex_mode,
				   base::Bps const &speed) override;

		/// @brief 发送。
		/// @param spans
		void Send(base::IEnumerable<base::ReadOnlySpan> const &spans) override;

		/// @brief 发送单个 span.
		/// @note 默认实现是基于 void Send(base::IEnumerable<base::ReadOnlySpan> const &spans).
		/// @param span
		void Send(base::ReadOnlySpan const &span) override;

		/// @brief 接收。
		/// @note 因为接收后需要解析，而解析需要数据完整且连续，所以必须接收一整个完整的以太网帧，
		/// 放到一个 span 中。
		/// @return
		base::ReadOnlySpan Receive() override;
	};

} // namespace bsp
