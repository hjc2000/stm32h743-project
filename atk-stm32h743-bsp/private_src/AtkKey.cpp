#include "AtkKey.h"

#pragma region Key0

bsp::Key0::Key0()
{
    auto options = DICreate_GpioPinOptions();
    options->SetDirection(bsp::IGpioPinDirection::Input);
    options->SetPullMode(bsp::IGpioPinPullMode::PullUp);
    options->SetSpeedLevel(3);
    options->SetWorkMode(bsp::IGpioPinWorkMode::Gpio);
    _pin = DI_GpioPinCollection().Get("PH3");
    _pin->Open(*options);
}

bool bsp::Key0::KeyIsDown()
{
    // 被按下是低电平，否则是高电平
    return !_pin->ReadPin();
}

#pragma endregion

#pragma region Key1

bsp::Key1::Key1()
{
    auto options = DICreate_GpioPinOptions();
    options->SetDirection(bsp::IGpioPinDirection::Input);
    options->SetPullMode(bsp::IGpioPinPullMode::PullUp);
    options->SetSpeedLevel(3);
    options->SetWorkMode(bsp::IGpioPinWorkMode::Gpio);
    _pin = DI_GpioPinCollection().Get("PH2");
    _pin->Open(*options);
}

bool bsp::Key1::KeyIsDown()
{
    // 被按下是低电平，否则是高电平
    return !_pin->ReadPin();
}

#pragma endregion
