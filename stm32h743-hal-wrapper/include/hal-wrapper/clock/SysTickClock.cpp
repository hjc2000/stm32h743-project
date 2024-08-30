#include "SysTickClock.h"
#include <hal-wrapper/clock/ClockSignal.h>
#include <task/TaskDelayer.h>

uint32_t hal::SysTickClock::Frequency() const
{
    // stm32h743 不支持 8 分频。
    uint32_t freq = HAL_RCC_GetSysClockFreq();
    return freq;
}

uint32_t hal::SysTickClock::ReloadValue() const
{
    uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
    return masked >> SysTick_LOAD_RELOAD_Pos;
}

uint32_t hal::SysTickClock::CurrentValue() const
{
    uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
    return masked >> SysTick_VAL_CURRENT_Pos;
}

extern "C"
{
    /// @brief 重写 __weak 的 HAL_Delay 函数
    /// @param ms 要延时的毫秒数。
    void HAL_Delay(uint32_t ms)
    {
        task::TaskDelayer::Instance().Delay(std::chrono::milliseconds{ms});
    }
}
