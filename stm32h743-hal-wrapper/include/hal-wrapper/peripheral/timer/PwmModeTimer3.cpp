#include"PwmModeTimer3.h"
#include<hal-wrapper/interrupt/Interrupt.h>
#include<hal-wrapper/interrupt/IsrManager.h>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>

void hal::PwmModeTimer3::OnPwmMspInitCallback(TIM_HandleTypeDef *handle)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	hal::Interrupt::SetPriority(IRQn_Type::TIM3_IRQn, 10, 0);
	hal::IsrManager::Instance().SetIrqHandler(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn), []()
	{
		HAL_TIM_IRQHandler(&hal::PwmModeTimer3::Instance().Handle());
	});

	hal::GpioPortB::Instance().EnableClock();
	hal::GpioPinConfig config;
	config.SetPin(hal::GpioPinConfig::PinEnum::Pin1);
	config.SetMode(hal::GpioPinConfig::ModeEnum::AlternateFunction_PushPull);
	config.SetPull(hal::GpioPinConfig::PullOption::PullUp);
	config.SetSpeed(hal::GpioPinConfig::SpeedOption::VeryHigh);
	config.SetAlternate(GPIO_AF2_TIM3);
	hal::GpioPortB::Instance().InitPin(config);
}

TIM_HandleTypeDef &hal::PwmModeTimer3::Handle()
{
	return _handle;
}

void hal::PwmModeTimer3::PwmInitialize(hal::UniversalTimerBaseConfig &config)
{
	_base_config = config;
	_handle.Instance = HardwareInstance();
	_handle.Init = _base_config.Handle();
	_handle.PWM_MspInitCallback = OnPwmMspInitCallback;
	HAL_TIM_PWM_Init(&_handle);
}
