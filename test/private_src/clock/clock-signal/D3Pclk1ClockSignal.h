#pragma once
#include <base/define.h>
#include <bsp-interface/clock/IClockSignal.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <hal.h>

namespace bsp
{
    class D3Pclk1ClockSignal :
        public bsp::IClockSignal
    {
    private:
        uint32_t _division_factor = 1;

    public:
        static_function D3Pclk1ClockSignal &Instance();

        /// @brief 时钟信号的名称。
        /// @return
        virtual std::string Name() const override;

        /// @brief 时钟信号的频率
        /// @return
        virtual base::MHz Frequency() const override;

        /// @brief 打开时钟信号。
        /// @note 有的时钟信号只有输入分频，没有输出分频，就使用本重载。
        /// @param input_division_factor
        virtual void Open(bsp::IClockSignal_InputDivisionFactor const &input_division_factor) override;
    };
} // namespace bsp
