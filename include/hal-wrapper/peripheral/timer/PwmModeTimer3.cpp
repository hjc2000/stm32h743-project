#include "PwmModeTimer3.h"
#include <bsp-interface/di/interrupt.h>

void hal::PwmModeTimer3::OnPwmMspInitCallback(TIM_HandleTypeDef *handle)
{
	__HAL_RCC_TIM3_CLK_ENABLE();

	bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn), 10);

	bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn),
											[]()
											{
												HAL_TIM_IRQHandler(&hal::PwmModeTimer3::Instance()._handle);
											});

	// auto pin = DI_GpioPinCollection().Get("PB1");
	// pin->OpenAsAlternateFunctionMode("timer3",
	// 								 bsp::IGpioPinPullMode::PullUp,
	// 								 bsp::IGpioPinDriver::PushPull);
}

void hal::PwmModeTimer3::PwmInitialize(hal::UniversalTimerBaseConfig &config)
{
	_base_config = config;
	_handle.Instance = HardwareInstance();
	_handle.Init = _base_config._handle;
	_handle.PWM_MspInitCallback = OnPwmMspInitCallback;
	HAL_TIM_PWM_Init(&_handle);
}
