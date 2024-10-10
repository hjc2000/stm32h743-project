#pragma once
#include <functional>
#include <hal-wrapper/clock/ClockSignal.h>
#include <hal-wrapper/peripheral/timer/TimerChannelEnum.h>
#include <hal-wrapper/peripheral/timer/UniversalTimerBaseConfig.h>
#include <hal-wrapper/peripheral/timer/UniversalTimerCompareOutputConfig.h>
#include <hal.h>
#include <stdexcept>
#include <task/Critical.h>

namespace hal
{
    class PwmModeTimer3
    {
    private:
        TIM_HandleTypeDef _handle{};
        hal::UniversalTimerBaseConfig _base_config{};

        TIM_TypeDef *HardwareInstance() const
        {
            return TIM3;
        }

        static_function void OnPwmMspInitCallback(TIM_HandleTypeDef *handle);

    public:
        static_function PwmModeTimer3 &Instance()
        {
            static PwmModeTimer3 o;
            return o;
        }

        /// @brief 初始化为 PWM 输出器。
        /// @param config
        void PwmInitialize(hal::UniversalTimerBaseConfig &config);

        /// @brief 配置指定的 PWM 输出通道。
        /// @param config
        /// @param channel
        void ConfigPwmChannel(hal::UniversalTimerCompareOutputConfig &config,
                              hal::TimerChannelEnum channel)
        {
            HAL_TIM_PWM_ConfigChannel(&_handle, &config._handle, static_cast<uint32_t>(channel));
        }

        /// @brief 启动指定的通道的 PWM 输出。
        /// @param channel
        void StartPwm(hal::TimerChannelEnum channel)
        {
            HAL_TIM_PWM_Start(&_handle, static_cast<uint32_t>(channel));
        }

        /// @brief 输入到分频器的时钟信号的频率
        /// @return
        uint32_t PrescalerInputClockSignalFrequency()
        {
            hal::ClockSignalConfig config = hal::ClockSignal::GetConfig();
            uint32_t pclk1_freq = hal::ClockSignal::Pclk1Freq();
            if (config._system_clk_config._hclk_config._apb1clk_config._input_divider == hal::Apb1ClkConfig::InputDivider::DIV1)
            {
                return pclk1_freq;
            }

            /* 如果 pclk1 对输入不是 1 分频，定时器内部会对来自 pclk1 的信号进行 2 倍频。 */
            return pclk1_freq * 2;
        }

        /// @brief 分频器的输出，输入到定时器的计数器中的时钟频率。
        /// @return
        uint32_t CounterFrequency()
        {
            return PrescalerInputClockSignalFrequency() / (_base_config.Prescaler() + 1);
        }

        HAL_TIM_ActiveChannel ActiveChannel()
        {
            return _handle.Channel;
        }

        void SetActiveChannel(HAL_TIM_ActiveChannel value)
        {
            _handle.Channel = value;
        }
    };
} // namespace hal
