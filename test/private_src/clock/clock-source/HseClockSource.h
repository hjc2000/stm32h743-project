#pragma once
#include <base/define.h>
#include <bsp-interface/clock/IClockSource.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <hal.h>

namespace bsp
{
    /// @brief hse 时钟源。需要外部晶振。
    class HseClockSource :
        public bsp::IClockSource
    {
    private:
        base::MHz _frequency{};
        IClockSource_State _state{};

    public:
        static_function HseClockSource &Instance();

#pragma region IClockSource
        /// @brief 本时钟源的名称。
        /// @return
        std::string Name() const override;

        /// @brief 打开时钟源。
        /// @note 有的时钟源非常简单，外部振荡器输入后，直接就输出同频率的时钟信号，
        /// 不支持分频和倍频。本重载用于这种简单的时钟源。
        /// @param crystal_frequency
        void Open(base::MHz const &crystal_frequency) override;

        /// @brief 设置为旁路。
        /// @param external_clock_frequency 外部时钟频率。
        void SetAsBypass(base::MHz external_clock_frequency) override;

        /// @brief 关闭时钟源。
        void Close() override;

        /// @brief 本时钟源当前的状态。
        /// @return
        IClockSource_State State() const override;

        /// @brief 本时钟源当前频率。
        /// @return
        base::MHz Frequency() const override;
#pragma endregion
    };
} // namespace bsp
