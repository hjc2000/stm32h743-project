#pragma once
#include <base/define.h>
#include <bsp-interface/clock/IClockSignal.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <hal.h>

namespace bsp
{
    class SysclkClockSignal :
        public bsp::IClockSignal
    {
    public:
        static_function SysclkClockSignal &Instance();

        /// @brief 时钟信号的名称。
        /// @return
        virtual std::string Name() const override;

        /// @brief 时钟信号的频率
        /// @return
        virtual base::MHz Frequency() const override;

        /// @brief 打开时钟信号。
        /// @param output_division_factor 输出分频系数。
        /// @param clock_source 时钟源。像 stm32 的系统时钟 sysclk，是时钟源后的第一个时钟信号，输入端连接着
        /// 各个时钟源，输出端供给各个子时钟信号。本参数用来选择类似 sysclk 这种时钟信号的时钟源。
        virtual void Open(bsp::IClockSignal_OutputDivisionFactor const &output_division_factor,
                          IClockSignal_ClockSource const &clock_source) override;
    };
} // namespace bsp
