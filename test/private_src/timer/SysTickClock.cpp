#include "SysTickClock.h"
#include "base/task/delay.h"
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>

extern "C"
{
	/// @brief 重写 __weak 的 HAL_Delay 函数
	/// @param ms 要延时的毫秒数。
	void HAL_Delay(uint32_t ms)
	{
		base::task::Delay(std::chrono::milliseconds{ms});
	}

	void SysTick_Handler()
	{
		// 读取一下 CTRL，SysTick 的溢出标志位会自动清除。
		SysTick->CTRL;
		HAL_IncTick();
		bsp::SysTickClock::Instance().AddSystemTime();

		if (!bsp::di::interrupt::GlobalInterruptIsDisabled() &&
			bsp::SysTickClock::Instance()._elapsed_handler)
		{
			bsp::SysTickClock::Instance()._elapsed_handler();
		}
	}
}

void bsp::SysTickClock::AddSystemTime()
{
	base::Seconds tick_interval{base::Hz{Frequency()}};
	base::Seconds elapsed_period = tick_interval * ReloadValue();
	_system_time += elapsed_period;
}

bsp::SysTickClock &bsp::SysTickClock::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<SysTickClock>
	{
	public:
		std::unique_ptr<SysTickClock> Create() override
		{
			return std::unique_ptr<SysTickClock>{new SysTickClock{}};
		}
	};

	Getter g;
	return g.Instance();
}

uint32_t bsp::SysTickClock::Frequency() const
{
	// stm32h743 不支持 8 分频。
	uint32_t freq = HAL_RCC_GetSysClockFreq();
	return freq;
}

uint32_t bsp::SysTickClock::ReloadValue() const
{
	uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
	return masked >> SysTick_LOAD_RELOAD_Pos;
}

uint32_t bsp::SysTickClock::CurrentValue() const
{
	uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
	return masked >> SysTick_VAL_CURRENT_Pos;
}

void bsp::SysTickClock::SetElapsedHandler(std::function<void()> func)
{
	bsp::di::interrupt::GlobalInterruptGuard g;
	_elapsed_handler = func;
}

base::Seconds bsp::SysTickClock::SystemTime() const
{
	return _system_time;
}
