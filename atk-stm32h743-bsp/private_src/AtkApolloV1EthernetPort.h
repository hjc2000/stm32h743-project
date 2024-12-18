#pragma once
#include <base/define.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/ethernet/phy/LAN8720A_EthernetPort.h>

namespace bsp
{
	/// @brief 以太网端口。
	class AtkApolloV1EthernetPort :
		public bsp::LAN8720A_EthernetPort
	{
	private:
		bsp::IEthernetController *_controller = &DI_EthernetController();

	protected:
		/// @brief 读 PHY 的寄存器
		/// @param register_index 寄存器索引。
		/// @return
		uint32_t ReadPHYRegister(uint32_t register_index) override;

		/// @brief 写 PHY 的寄存器。
		/// @param register_index 寄存器索引。
		/// @param value
		void WritePHYRegister(uint32_t register_index, uint32_t value) override;

	public:
		static_function AtkApolloV1EthernetPort &Instance();

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

		/// @brief 复位 PHY 芯片。
		void ResetPHY() override;

		/// @brief 发送。
		/// @param spans
		void Send(base::IEnumerable<base::ReadOnlySpan> const &spans) override;

		/// @brief 接收。
		/// @return
		base::IEnumerable<base::ReadOnlySpan> const &Receive() override;
	};
} // namespace bsp
