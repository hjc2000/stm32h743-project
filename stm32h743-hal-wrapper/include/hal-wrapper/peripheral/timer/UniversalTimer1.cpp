#include"UniversalTimer1.h"
#include<hal-wrapper/interrupt/Interrupt.h>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>

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

#pragma region 作为基本定时器
void hal::UniversalTimer1::OnBaseMspInitCallback(TIM_HandleTypeDef *handle)
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

void hal::UniversalTimer1::BaseInitialize(hal::UniversalTimerBaseConfig &config)
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
void hal::UniversalTimer1::OnPwmMspInitCallback(TIM_HandleTypeDef *handle)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	hal::Interrupt::SetPriority(IRQn_Type::TIM3_IRQn, 10, 0);
	hal::Interrupt::EnableIRQ(IRQn_Type::TIM3_IRQn);

	hal::GpioPortB::Instance().EnableClock();
	hal::GpioPinConfig config;
	config._mode = hal::GpioPinMode::AlternateFunction_PushPull;
	config._pull_mode = hal::GpioPinPull::PullUp;
	config._speed = hal::GpioPinSpeed::VeryHigh;
	config._alternate = GPIO_AF2_TIM3;
	hal::GpioPortB::Instance().InitPin(hal::GpioPin::Pin1, config);
}

void hal::UniversalTimer1::PwmInitialize(hal::UniversalTimerBaseConfig &config)
{
	_base_config = config;
	_handle.Instance = HardwareInstance();
	_handle.Init = _base_config.Handle();
	_handle.PWM_MspInitCallback = OnPwmMspInitCallback;
	HAL_TIM_PWM_Init(&_handle);
}
#pragma endregion
