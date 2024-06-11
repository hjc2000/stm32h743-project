#include"IsrManager.h"
#include<hal-wrapper/interrupt/Interrupt.h>

using namespace hal;
using namespace bsp;

void hal::InterruptSwitch::DisableInterrupt(uint32_t irq) noexcept
{
	hal::Interrupt::DisableIRQ(static_cast<IRQn_Type>(irq));
}

void hal::InterruptSwitch::EnableInterrupt(uint32_t irq) noexcept
{
	hal::Interrupt::EnableIRQ(static_cast<IRQn_Type>(irq));
}

bsp::IsrManager &hal::GetIsrManager()
{
	static bsp::IsrManager manager { hal::InterruptSwitch::Instance() };
	return manager;
}

extern "C"
{
	void TIM3_IRQHandler()
	{
		try
		{
			auto func = hal::GetIsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn));
			if (func)
			{
				func();
			}
		}
		catch (...)
		{

		}
	}
}
