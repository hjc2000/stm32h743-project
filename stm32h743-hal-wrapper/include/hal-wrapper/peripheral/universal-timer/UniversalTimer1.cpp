#include"UniversalTimer1.h"
#include<hal-wrapper/interrupt/Interrupt.h>

extern "C"
{
	void TIM3_IRQHandler()
	{
		HAL_TIM_IRQHandler(&hal::UniversalTimer1::Instance().Handle());
	}
}

void hal::UniversalTimer1::OnMspInitCallback(TIM_HandleTypeDef *handle)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	hal::Interrupt::SetPriority(IRQn_Type::TIM3_IRQn, 10, 0);
	hal::Interrupt::EnableIRQ(IRQn_Type::TIM3_IRQn);
}

void hal::UniversalTimer1::OnPeriodElapsed(TIM_HandleTypeDef *handle)
{
	if (hal::UniversalTimer1::Instance()._period_elapsed_callback)
	{
		hal::UniversalTimer1::Instance()._period_elapsed_callback();
	}
}

TIM_HandleTypeDef &hal::UniversalTimer1::Handle()
{
	return _handle;
}

void hal::UniversalTimer1::Initialize(hal::UniversalTimerConfig const &config)
{
	_config = config;
	_handle.Instance = HardwareInstance();
	_handle.Init = _config.Handle();
	_handle.Base_MspInitCallback = OnMspInitCallback;
	HAL_TIM_Base_Init(&_handle);

	_handle.PeriodElapsedCallback = OnPeriodElapsed;

	HAL_TIM_Base_Start_IT(&_handle);
}
