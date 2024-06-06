#pragma once
#include<base/HandleWrapper.h>
#include<hal.h>

namespace hal
{
	class UniversalTimer1 :
		public base::HandleWrapper<TIM_HandleTypeDef>
	{
	private:
		TIM_HandleTypeDef _handle { };

	public:
		TIM_HandleTypeDef &Handle() override;

	};
}