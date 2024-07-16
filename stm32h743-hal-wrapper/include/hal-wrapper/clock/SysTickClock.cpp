#include "SysTickClock.h"
#include <hal-wrapper/clock/ClockSignal.h>

uint32_t hal::SysTickClock::Frequency()
{
	// stm32h743 不支持 8 分频。
	uint32_t freq = HAL_RCC_GetSysClockFreq();
	return freq;
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
