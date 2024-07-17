#include "IsrManager.h"
#include <hal-wrapper/interrupt/Interrupt.h>
#include <hal-wrapper/interrupt/InterruptSwitch.h>

bsp::IsrManager &hal::GetIsrManager()
{
	static bsp::IsrManager manager{InterruptSwitch::Instance()};
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
