#pragma once
#include <base/define.h>
#include <base/unit/Hz.h>
#include <base/unit/Seconds.h>
#include <bsp-interface/timer/ISysTick.h>
#include <chrono>
#include <hal.h>
#include <stdint.h>

extern "C"
{
    void SysTick_Handler();
}

namespace bsp
{
    /// @brief stm32h743 的 SysTick 不支持设置时钟源。他只能与 CPU 相同频率，
    /// 即使用系统时钟信号。
    class SysTickClock :
        public bsp::ISysTick
    {
    private:
        SysTickClock() = default;

        std::function<void()> _elapsed_handler;
        base::Seconds _system_time{0};

        friend void ::SysTick_Handler();

        void AddSystemTime();

    public:
        static_function SysTickClock &Instance();

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

        /// @brief 定时时间到处理函数。
        /// @warning 实现者需要将本函数实现为线程安全和可重入。最简单的方法就是在本函数的
        /// 操作期间禁用全局中断。
        /// @param func
        void SetElapsedHandler(std::function<void()> func) override;

        base::Seconds SystemTime() const;
    };
} // namespace bsp
