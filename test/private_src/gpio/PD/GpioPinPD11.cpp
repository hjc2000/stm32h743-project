#include "GpioPinPD11.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPD11::Port()
{
    return GPIOD;
}

uint32_t bsp::GpioPinPD11::Pin()
{
    return GPIO_PIN_11;
}

std::string bsp::GpioPinPD11::PinName() const
{
    return "PD11";
}
