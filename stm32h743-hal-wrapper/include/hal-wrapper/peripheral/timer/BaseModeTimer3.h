#pragma once
#include <base/define.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/interrupt.h>
#include <functional>
#include <hal-wrapper/peripheral/timer/TimerChannelEnum.h>
#include <hal-wrapper/peripheral/timer/UniversalTimerBaseConfig.h>
#include <hal-wrapper/peripheral/timer/UniversalTimerCompareOutputConfig.h>
#include <hal.h>
#include <stdexcept>

namespace hal
{
	class BaseModeTimer3
	{
	private:
		hal::UniversalTimerBaseConfig _base_config{};
		std::function<void()> _period_elapsed_callback{};

		TIM_TypeDef *HardwareInstance() const
		{
			return TIM3;
		}

		static_function void OnBaseMspInitCallback(TIM_HandleTypeDef *handle);
		static_function void OnPeriodElapsed(TIM_HandleTypeDef *handle);

	public:
		TIM_HandleTypeDef _handle{};

		static_function BaseModeTimer3 &Instance()
		{
			static BaseModeTimer3 o;
			return o;
		}

		/// @brief 初始化为基本定时器。
		/// @param config
		void BaseInitialize(hal::UniversalTimerBaseConfig &config);

		void SetPeriodElapsedCallback(std::function<void()> func)
		{
			DI_DoGlobalCriticalWork(
				[&]()
				{
					_period_elapsed_callback = func;
				});
		}

		/// @brief 输入到分频器的时钟信号的频率
		/// @return
		uint32_t PrescalerInputClockSignalFrequency()
		{
			// hal::ClockSignalConfig config = hal::ClockSignal::GetConfig();
			uint32_t pclk1_freq = static_cast<int64_t>(DI_ClockSignalCollection().Get("pclk1")->Frequency());
			// if (config._system_clk_config._hclk_config._apb1clk_config._input_divider == hal::Apb1ClkConfig::InputDivider::DIV1)
			// {
			//     return pclk1_freq;
			// }

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
