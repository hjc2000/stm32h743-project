#include "SysTickClock.h"
#include <hal-wrapper/clock/ClockSignal.h>

hal::SysTickClock::SysTickClockSourceOption hal::SysTickClock::ClockSource()
{
	uint32_t masked = SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk;
	if (masked)
	{
		return SysTickClockSourceOption::HCLK;
	}

	return SysTickClockSourceOption::HCLK_DIV8;
}

void hal::SysTickClock::SetClockSource(hal::SysTickClock::SysTickClockSourceOption value)
{
	HAL_SYSTICK_CLKSourceConfig(static_cast<uint32_t>(value));
}

uint32_t hal::SysTickClock::Frequency()
{
	return HAL_RCC_GetSysClockFreq();
}

uint32_t hal::SysTickClock::ReloadValue()
{
	uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
	return masked >> SysTick_LOAD_RELOAD_Pos;
}

uint32_t hal::SysTickClock::CurrentValue()
{
	uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
	return masked >> SysTick_VAL_CURRENT_Pos;
}
