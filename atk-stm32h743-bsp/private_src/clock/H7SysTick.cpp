#include"H7SysTick.h"
#include<ClockSignal.h>

using namespace bsp;

bool H7SysTick::CountFlag()
{
	uint32_t masked = SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk;
	return masked >> SysTick_CTRL_COUNTFLAG_Pos;
}

H7SysTick::SysTickClockSource H7SysTick::ClockSource()
{
	uint32_t masked = SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk;
	if (masked)
	{
		return SysTickClockSource::HCLK;
	}

	return SysTickClockSource::HCLK_DIV8;
}

void H7SysTick::SetClockSource(H7SysTick::SysTickClockSource value)
{
	HAL_SYSTICK_CLKSourceConfig(static_cast<uint32_t>(value));
}

uint32_t H7SysTick::Frequency()
{
	return HAL_RCC_GetSysClockFreq();
}

uint32_t H7SysTick::ReloadValue()
{
	uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
	return masked >> SysTick_LOAD_RELOAD_Pos;
}

uint32_t H7SysTick::CurrentValue()
{
	uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
	return masked >> SysTick_VAL_CURRENT_Pos;
}
