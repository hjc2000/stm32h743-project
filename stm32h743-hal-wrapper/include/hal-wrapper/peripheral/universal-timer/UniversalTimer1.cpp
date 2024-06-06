#include"UniversalTimer1.h"

extern "C"
{
	void TIM3_IRQHandler()
	{
		HAL_TIM_IRQHandler(&hal::UniversalTimer1::Instance().Handle());
	}
}

TIM_HandleTypeDef &hal::UniversalTimer1::Handle()
{
	return _handle;
}
