#pragma once
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/ISysTick.h>
#include <chrono>
#include <hal.h>
#include <stdint.h>

namespace hal
{
    /// @brief stm32h743 的 SysTick 不支持设置时钟源。他只能与 CPU 相同频率，
    /// 即使用系统时钟信号。
    class SysTickClock : public bsp::ISysTick
    {
    private:
        SysTickClock() = default;

    public:
        static SysTickClock &Instance()
        {
            class Getter : public base::SingletonGetter<SysTickClock>
            {
            public:
                std::unique_ptr<SysTickClock> Create() override
                {
                    return std::unique_ptr<SysTickClock>{new SysTickClock{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }

        /// @brief 获取当前的实际的 Systic 的频率。
        /// @note stm32h743 的 Systic 永远只能与 CPU 频率相同，即只能使用系统时钟信号
        /// 作为时钟源，无法使用系统时钟信号 8 分频后的信号作为时钟源。
        /// @return
        uint32_t Frequency() const override;

        /// @brief 获取 SysTick 的 LOAD 寄存器的 RELOAD 部分的值。
        /// RELOAD 占据 LOAD 寄存器的低 23 位。
        /// @note RELOAD 是用来在计数值递减到 0 后，下一个时钟周期装载到计数器中的。
        /// @note 使用了 freertos 后，重装载值会被 freertos 设置。
        /// @return
        uint32_t ReloadValue() const override;

        /// @brief 获取 SysTick 的 VAL 寄存器的 CURRENT 部分的值。
        /// @note CURRENT 部分占据了寄存器的低 23 位，表示当前计数值。
        /// @warning CURRENT 可读可写。只不过这里的写并不是如你所愿地将数据赋予 CURRENT。
        /// 写入 CURRENT 会导致 CURRENT 清零，同时会导致 CTRL 寄存器的 COUNTFLAG 位清零。
        ///
        /// @return 当前计数值
        uint32_t CurrentValue() const override;
    };
} // namespace hal
