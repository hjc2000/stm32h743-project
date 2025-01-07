#pragma once
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/ethernet/phy/IPhyController.h>

namespace bsp
{
	class PhyController :
		public bsp::IPhyController
	{
	private:
		bsp::IEthernetController *_controller = &DI_EthernetController();

	public:
		/// @brief 读 PHY 的寄存器
		/// @param register_index 寄存器索引。
		/// @return
		virtual uint32_t ReadRegister(uint32_t register_index) override;

		/// @brief 写 PHY 的寄存器。
		/// @param register_index 寄存器索引。
		/// @param value
		virtual void WriteRegister(uint32_t register_index, uint32_t value) override;

		/// @brief 通过 PHY 的复位引脚将 PHY 复位。
		virtual void HardwareReset() override;
	};
} // namespace bsp
