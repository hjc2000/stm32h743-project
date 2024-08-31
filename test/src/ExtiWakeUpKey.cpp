#include "ExtiWakeUpKey.h"

using namespace bsp;

ExtiWakeUpKey::ExtiWakeUpKey()
{
    auto options = DICreate_GpioPinOptions();
    options->SetDirection(bsp::IGpioPinDirection::Input);
    options->SetPullMode(bsp::IGpioPinPullMode::PullUp);
    options->SetSpeedLevel(3);
    options->SetTriggerEdge(bsp::IGpioPinTriggerEdge::RisingEdge);
    options->SetWorkMode(bsp::IGpioPinWorkMode::Gpio);
    _pin = DI_GpioPinCollection().Get("PH3");
    _pin->Open(*options);

    _pin->RegisterInterruptCallback(
        [&]()
        {
            // 这是在中断函数中，禁止使用 Delayer 进行延时。
            DI_SysTick().Delay(std::chrono::milliseconds{20});
            _is_pressed = _pin->ReadPin();
        });
}
