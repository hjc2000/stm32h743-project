#include"IsrManager.h"
#include<hal-wrapper/interrupt/Interrupt.h>

using namespace hal;
using namespace bsp;

bsp::IsrManager &hal::GetIsrManager()
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

		void DisableInterrupt(uint32_t irq) noexcept override
		{
			hal::Interrupt::DisableIRQ(static_cast<IRQn_Type>(irq));
		}

		void EnableInterrupt(uint32_t irq) noexcept override
		{
			hal::Interrupt::EnableIRQ(static_cast<IRQn_Type>(irq));
		}
	};

	static bsp::IsrManager manager { InterruptSwitch::Instance() };
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
