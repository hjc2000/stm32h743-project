#pragma once
#include<base/HandleWrapper.h>
#include<functional>
#include<hal.h>
#include<hal-wrapper/clock/ClockSignal.h>
#include<hal-wrapper/peripheral/timer/TimerChannelEnum.h>
#include<hal-wrapper/peripheral/timer/UniversalTimerBaseConfig.h>
#include<hal-wrapper/peripheral/timer/UniversalTimerCompareOutputConfig.h>
#include<stdexcept>
#include<task/Critical.h>

namespace hal
{
	class PwmModeTimer3 :
		public base::HandleWrapper<TIM_HandleTypeDef>
	{
	private:
		TIM_HandleTypeDef _handle { };
		hal::UniversalTimerBaseConfig _base_config { };

		TIM_TypeDef *HardwareInstance() const
		{
			return TIM3;
		}

		static void OnPwmMspInitCallback(TIM_HandleTypeDef *handle);

	public:
		static PwmModeTimer3 &Instance()
		{
			static PwmModeTimer3 o;
			return o;
		}

		TIM_HandleTypeDef &Handle() override;

		/// <summary>
		///		初始化为 PWM 输出器。
		/// </summary>
		/// <param name="config"></param>
		void PwmInitialize(hal::UniversalTimerBaseConfig &config);

		/// <summary>
		///		配置指定的 PWM 输出通道。
		/// </summary>
		/// <param name="config"></param>
		/// <param name="channel"></param>
		void ConfigPwmChannel(
			hal::UniversalTimerCompareOutputConfig &config,
			hal::TimerChannelEnum channel
		)
		{
			HAL_TIM_PWM_ConfigChannel(&_handle, config, static_cast<uint32_t>(channel));
		}

		/// <summary>
		///		启动指定的通道的 PWM 输出。
		/// </summary>
		/// <param name="channel"></param>
		void StartPwm(hal::TimerChannelEnum channel)
		{
			HAL_TIM_PWM_Start(&_handle, static_cast<uint32_t>(channel));
		}

		/// <summary>
		///		输入到分频器的时钟信号的频率
		/// </summary>
		/// <returns></returns>
		uint32_t PrescalerInputClockSignalFrequency()
		{
			hal::ClockSignalConfig config = hal::ClockSignal::GetConfig();
			uint32_t pclk1_freq = hal::ClockSignal::Pclk1Freq();
			if (config._system_clk_config._hclk_config._apb1clk_config._input_divider
				== hal::Apb1ClkConfig::InputDivider::DIV1)
			{
				return pclk1_freq;
			}

			/* 如果 pclk1 对输入不是 1 分频，定时器内部会对来自 pclk1 的信号进行 2 倍频。 */
			return pclk1_freq * 2;
		}

		/// <summary>
		///		分频器的输出，输入到定时器的计数器中的时钟频率。
		/// </summary>
		/// <returns></returns>
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
}
