#pragma once
#include<bsp-interface/interrupt/IsrManager.h>

namespace hal
{
	class InterruptSwitch :
		public bsp::IInterruptSwitch
	{
	private:
		InterruptSwitch() = default;

	public:
		static InterruptSwitch &Instance()
		{
			static InterruptSwitch o;
			return o;
		}

		void DisableInterrupt(uint32_t irq) noexcept override;
		void EnableInterrupt(uint32_t irq) noexcept override;
	};

	bsp::IsrManager &GetIsrManager();
}