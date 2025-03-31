#include "GpioPinPH2.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPH2::Port()
{
    return GPIOH;
}

uint32_t bsp::GpioPinPH2::Pin()
{
    return GPIO_PIN_2;
}

std::string bsp::GpioPinPH2::PinName() const
{
    return "PH2";
}
