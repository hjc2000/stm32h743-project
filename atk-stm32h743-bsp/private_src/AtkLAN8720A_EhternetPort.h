#pragma once
#include <base/define.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/ethernet/phy/LAN8720A_EthernetPort.h>
#include <bsp-interface/ethernet/phy/YT8512C_EthernetPort.h>

namespace bsp
{
	/// @brief 以太网端口。
	class AtkLAN8720A_EhternetPort :
		public bsp::YT8512C_EthernetPort
	{
	private:
		bsp::IEthernetController *_controller = &DI_EthernetController();

	public:
		static_function AtkLAN8720A_EhternetPort &Instance();

		/// @brief 以太网控制器的名称。
		/// @return
		std::string Name() const override;

		/// @brief 打开以太网端口。
		/// @param mac MAC 地址。
		void Open(base::Mac const &mac) override;

		/// @brief 读 PHY 的寄存器
		/// @param register_index 寄存器索引。
		/// @return
		uint32_t ReadPHYRegister(uint32_t register_index) override;

		/// @brief 写 PHY 的寄存器。
		/// @param register_index 寄存器索引。
		/// @param value
		void WritePHYRegister(uint32_t register_index, uint32_t value) override;

		/// @brief 复位 PHY 芯片。
		void ResetPHY() override;

		/// @brief 发送。
		/// @param span
		void Send(base::ReadOnlySpan const &span) override;
	};
} // namespace bsp