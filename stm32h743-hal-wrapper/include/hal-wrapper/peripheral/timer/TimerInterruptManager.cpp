#include"TimerInterruptManager.h"
#include<hal-wrapper/interrupt/Interrupt.h>

using namespace hal;
using namespace bsp;

std::map<uint32_t, std::function<void()>> &hal::TimerInterruptManager::HandleMap()
{
	return _handler_map;
}

void hal::TimerInterruptManager::DisableTimerInterrupt(uint32_t timer_id) noexcept
{
	switch (timer_id)
	{
	case 2:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::TIM2_IRQn);
			break;
		}
	case 3:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::TIM3_IRQn);
			break;
		}
	case 4:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::TIM4_IRQn);
			break;
		}
	}
}

void hal::TimerInterruptManager::EnableTimerInterrupt(uint32_t timer_id) noexcept
{
	switch (timer_id)
	{
	case 2:
		{
			hal::Interrupt::EnableIRQ(IRQn_Type::TIM2_IRQn);
			break;
		}
	case 3:
		{
			hal::Interrupt::EnableIRQ(IRQn_Type::TIM3_IRQn);
			break;
		}
	case 4:
		{
			hal::Interrupt::EnableIRQ(IRQn_Type::TIM4_IRQn);
			break;
		}
	}
}
