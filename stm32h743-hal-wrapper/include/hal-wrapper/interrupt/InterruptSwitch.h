#pragma once
#include <bsp-interface/interrupt/IInterruptSwitch.h>
#include <hal-wrapper/interrupt/Interrupt.h>
#include <hal.h>

namespace hal
{
	class InterruptSwitch
		: public bsp::IInterruptSwitch
	{
	private:
		InterruptSwitch() = default;

	public:
		static InterruptSwitch &Instance()
		{
			static InterruptSwitch o;
			return o;
		}

		void DisableInterrupt(uint32_t irq) noexcept override
		{
			hal::Interrupt::DisableIRQ(static_cast<IRQn_Type>(irq));
		}

		void EnableInterrupt(uint32_t irq) noexcept override
		{
			hal::Interrupt::EnableIRQ(static_cast<IRQn_Type>(irq));
		}

		/// @brief 禁止全局中断
		void DisableGlobalInterrupt() noexcept override
		{
			__disable_irq();
		}

		/// @brief 启用全局中断
		void EnableGlobalInterrupt() noexcept override
		{
			__enable_irq();
		}
	};
}