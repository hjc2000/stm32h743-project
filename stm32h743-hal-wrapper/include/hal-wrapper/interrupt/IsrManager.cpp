#include <bsp-interface/di.h>
#include <hal-wrapper/interrupt/Interrupt.h>
#include <hal-wrapper/interrupt/InterruptSwitch.h>

extern "C"
{
	void TIM3_IRQHandler()
	{
		try
		{
			auto func = DI_IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn));
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
