#include"IsrManager.h"
#include<hal-wrapper/interrupt/Interrupt.h>

using namespace hal;
using namespace bsp;

std::map<uint32_t, std::function<void()>> &hal::IsrManager::HandlerMap()
{
	return _handler_map;
}

void hal::IsrManager::DisableInterrupt(uint32_t irq) noexcept
{
	hal::Interrupt::DisableIRQ(static_cast<IRQn_Type>(irq));
}

void hal::IsrManager::EnableInterrupt(uint32_t irq) noexcept
{
	hal::Interrupt::EnableIRQ(static_cast<IRQn_Type>(irq));
}

extern "C"
{
	void TIM3_IRQHandler()
	{
		try
		{
			auto func = IsrManager::Instance().GetIrqHandlerFromIsr(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn));
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
