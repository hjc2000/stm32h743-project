#include "GpioPinPD5.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPD5::Port()
{
    return GPIOD;
}

uint32_t bsp::GpioPinPD5::Pin()
{
    return GPIO_PIN_5;
}

std::string bsp::GpioPinPD5::PinName() const
{
    return "PD5";
}
