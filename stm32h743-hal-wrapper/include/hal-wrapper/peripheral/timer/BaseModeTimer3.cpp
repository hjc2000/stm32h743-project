#include "BaseModeTimer3.h"
#include <hal-wrapper/peripheral/gpio/GpioPort.h>
#include <bsp-interface/di.h>

void hal::BaseModeTimer3::OnBaseMspInitCallback(TIM_HandleTypeDef *handle)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	DI_InterruptSwitch().EnableInterrupt(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn), 10);
}

void hal::BaseModeTimer3::OnPeriodElapsed(TIM_HandleTypeDef *handle)
{
	if (hal::BaseModeTimer3::Instance()._period_elapsed_callback)
	{
		hal::BaseModeTimer3::Instance()._period_elapsed_callback();
	}
}

TIM_HandleTypeDef &hal::BaseModeTimer3::Handle()
{
	return _handle;
}

void hal::BaseModeTimer3::BaseInitialize(hal::UniversalTimerBaseConfig &config)
{
	_base_config = config;
	_handle.Instance = HardwareInstance();
	_handle.Init = _base_config.Handle();
	_handle.Base_MspInitCallback = OnBaseMspInitCallback;
	HAL_TIM_Base_Init(&_handle);

	_handle.PeriodElapsedCallback = OnPeriodElapsed;

	HAL_TIM_Base_Start_IT(&_handle);
}
