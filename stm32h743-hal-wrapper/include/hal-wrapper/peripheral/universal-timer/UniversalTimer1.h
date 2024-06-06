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
		UniversalTimer1 &Instance()
		{
			static UniversalTimer1 o;
			return o;
		}


		TIM_HandleTypeDef &Handle() override;

	};
}