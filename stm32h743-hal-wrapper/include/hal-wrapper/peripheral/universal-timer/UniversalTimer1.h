#pragma once
#include<base/HandleWrapper.h>
#include<hal.h>
#include<hal-wrapper/peripheral/universal-timer/UniversalTimerConfig.h>

extern "C"
{
	void TIM3_IRQHandler();
}

namespace hal
{
	/// <summary>
	///		通用定时器
	/// </summary>
	class UniversalTimer1 :
		public base::HandleWrapper<TIM_HandleTypeDef>
	{
	private:
		friend void ::TIM3_IRQHandler();
		TIM_HandleTypeDef _handle { };
		hal::UniversalTimerConfig _config { };

		TIM_TypeDef *HardwareInstance()
		{
			return TIM3;
		}

	public:
		static UniversalTimer1 &Instance()
		{
			static UniversalTimer1 o;
			return o;
		}


		TIM_HandleTypeDef &Handle() override;

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