#include"Timer3.h"
#include<hal-wrapper/interrupt/Interrupt.h>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>

extern "C"
{
	void TIM3_IRQHandler()
	{
		HAL_TIM_IRQHandler(&hal::Timer3::Instance().Handle());
	}
}

TIM_HandleTypeDef &hal::Timer3::Handle()
{
	return _handle;
}

#pragma region 作为基本定时器
void hal::Timer3::OnBaseMspInitCallback(TIM_HandleTypeDef *handle)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	hal::Interrupt::SetPriority(IRQn_Type::TIM3_IRQn, 10, 0);
	hal::Interrupt::EnableIRQ(IRQn_Type::TIM3_IRQn);
}

void hal::Timer3::OnPeriodElapsed(TIM_HandleTypeDef *handle)
{
	if (hal::Timer3::Instance()._period_elapsed_callback)
	{
		hal::Timer3::Instance()._period_elapsed_callback();
	}
}

void hal::Timer3::BaseInitialize(hal::UniversalTimerBaseConfig &config)
{
	_base_config = config;
	_handle.Instance = HardwareInstance();
	_handle.Init = _base_config.Handle();
	_handle.Base_MspInitCallback = OnBaseMspInitCallback;
	HAL_TIM_Base_Init(&_handle);

	_handle.PeriodElapsedCallback = OnPeriodElapsed;

	HAL_TIM_Base_Start_IT(&_handle);
}
#pragma endregion

#pragma region 作为PWM输出器
void hal::Timer3::OnPwmMspInitCallback(TIM_HandleTypeDef *handle)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	hal::Interrupt::SetPriority(IRQn_Type::TIM3_IRQn, 10, 0);
	hal::Interrupt::EnableIRQ(IRQn_Type::TIM3_IRQn);

	hal::GpioPortB::Instance().EnableClock();
	hal::GpioPinConfig config;
	config.SetPin(hal::GpioPinConfig::PinEnum::Pin1);
	config.SetMode(hal::GpioPinConfig::ModeEnum::AlternateFunction_PushPull);
	config.SetPull(hal::GpioPinConfig::PullOption::PullUp);
	config.SetSpeed(hal::GpioPinConfig::SpeedOption::VeryHigh);
	config.SetAlternate(GPIO_AF2_TIM3);
	hal::GpioPortB::Instance().InitPin(config);
}

void hal::Timer3::PwmInitialize(hal::UniversalTimerBaseConfig &config)
{
	_base_config = config;
	_handle.Instance = HardwareInstance();
	_handle.Init = _base_config.Handle();
	_handle.PWM_MspInitCallback = OnPwmMspInitCallback;
	HAL_TIM_PWM_Init(&_handle);
}
#pragma endregion
