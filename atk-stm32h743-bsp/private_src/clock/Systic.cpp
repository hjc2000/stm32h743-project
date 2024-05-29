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

uint32_t Systic::ReloadNum()
{
	uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
	return masked >> SysTick_LOAD_RELOAD_Pos;
}

uint32_t Systic::CurrentValue()
{
	uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
	return masked >> SysTick_VAL_CURRENT_Pos;
}

void Systic::NopLoopDelayForTicks(uint32_t tick_count)
{
	/* 这里不禁用操作系统的调度。不要让此函数耦合性太强。
	* 如果需要的话，禁用操作系统的调度这个操作应该放到本函数外，调用者自己执行。
	*/
	uint32_t old_tick = CurrentValue();
	uint32_t total_tick = 0;
	while (1)
	{
		uint32_t now_tick = CurrentValue();
		if (old_tick == now_tick)
		{
			// CPU 太快了，导致再次进入循环后 old_tick == now_tick
			continue;
		}

		uint32_t delta_tick = 0;
		if (old_tick > now_tick)
		{
			// 因为是减计数，所以此时没有发生环绕
			delta_tick = old_tick - now_tick;
		}
		else
		{
			// 发生了环绕
			// delta_tick = old_tick - (now_tick - reload)
			// delta_tick = old_tick - now_tick + reload
			delta_tick = old_tick - now_tick + ReloadNum();
		}

		total_tick += delta_tick;
		if (total_tick >= tick_count)
		{
			return;
		}

		old_tick = now_tick;
	}
}

void Systic::NopLoopDelay(std::chrono::microseconds microseconds)
{
	uint32_t freq = Frequency();

	/*
	* 时钟周期 T = 1 / freq
	* 要延时的周期数
	*	N = microseconds / 1e6 / T
	*	N = microseconds / 1e6 * freq
	* 因为 freq 较大，所以调整为
	*	N = freq / 1e6 * microseconds
	* 这样可避免溢出
	*/
	NopLoopDelayForTicks(freq / (uint32_t)1e6 * microseconds.count());
}

void Systic::NopLoopDelay(std::chrono::milliseconds milliseconds)
{
	NopLoopDelay(std::chrono::microseconds { milliseconds });
}

void Systic::NopLoopDelay(std::chrono::seconds seconds)
{
	NopLoopDelay(std::chrono::milliseconds { seconds });
}
