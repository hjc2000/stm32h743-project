#include"Systic.h"
#include<ClockSignal.h>

using namespace bsp;

bool Systic::CountFlag()
{
	uint32_t masked = SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk;
	return masked >> SysTick_CTRL_COUNTFLAG_Pos;
}

SysticClockSource Systic::ClockSource()
{
	uint32_t masked = SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk;
	if (masked)
	{
		return SysticClockSource::HCLK;
	}

	return SysticClockSource::HCLK_DIV8;
}

void Systic::SetClockSource(SysticClockSource value)
{
	HAL_SYSTICK_CLKSourceConfig(static_cast<uint32_t>(value));
}

uint32_t Systic::Frequency()
{
	return HAL_RCC_GetSysClockFreq();
}

uint32_t Systic::ReloadValue()
{
	uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
	return masked >> SysTick_LOAD_RELOAD_Pos;
}

uint32_t Systic::CurrentValue()
{
	uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
	return masked >> SysTick_VAL_CURRENT_Pos;
}
