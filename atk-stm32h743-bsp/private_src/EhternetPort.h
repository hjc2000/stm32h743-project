#pragma once
#include <base/define.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/ethernet/phy/LAN8720A_EthernetPort.h>

namespace bsp
{
	/// @brief 以太网端口。
	class EhternetPort :
		public bsp::LAN8720A_EthernetPort
	{
	private:
		bsp::IEthernetController *_controller = &DI_EthernetController();

		void ChipInitialize();

	public:
		static_function EhternetPort &Instance();

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

		/// @brief 获取此网口的双工模式。
		/// @return
		bsp::Ethernet_DuplexMode DuplexMode() override;

		/// @brief 获取此网口的速度。
		/// @return
		base::Bps Speed() override;
	};
} // namespace bsp
