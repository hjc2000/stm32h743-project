#pragma once
#include<base/HandleWrapper.h>
#include<hal.h>
#include<hal-wrapper/peripheral/universal-timer/UniversalTimerConfig.h>

namespace hal
{
	class UniversalTimer1 :
		public base::HandleWrapper<TIM_HandleTypeDef>
	{
	private:
		TIM_HandleTypeDef _handle { };
		hal::UniversalTimerConfig _config { };

		TIM_TypeDef *HardwareInstance()
		{
			return TIM3;
		}

	public:
		UniversalTimer1 &Instance()
		{
			static UniversalTimer1 o;
			return o;
		}


		TIM_HandleTypeDef &Handle() override;

	};
}