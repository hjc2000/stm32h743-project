#include "PwmModeTimer3.h"
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>

void hal::PwmModeTimer3::OnPwmMspInitCallback(TIM_HandleTypeDef *handle)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    DI_InterruptSwitch().EnableInterrupt(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn), 10);
    DI_IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn),
                           []()
                           {
                               HAL_TIM_IRQHandler(&hal::PwmModeTimer3::Instance()._handle);
                           });

    auto options = DICreate_GpioPinOptions();
    options->SetAlternateFunction("timer3");
    options->SetDriver(bsp::IGpioPinDriver::PushPull);
    options->SetPullMode(bsp::IGpioPinPullMode::PullUp);
    options->SetSpeedLevel(3);
    options->SetWorkMode(bsp::IGpioPinWorkMode::AlternateFunction);

    auto pin = DI_GpioPinCollection().Get("PB1");
    pin->Open(*options);
}

void hal::PwmModeTimer3::PwmInitialize(hal::UniversalTimerBaseConfig &config)
{
    _base_config = config;
    _handle.Instance = HardwareInstance();
    _handle.Init = _base_config._handle;
    _handle.PWM_MspInitCallback = OnPwmMspInitCallback;
    HAL_TIM_PWM_Init(&_handle);
}
