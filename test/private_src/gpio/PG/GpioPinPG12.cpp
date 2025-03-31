#include "GpioPinPG12.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPG12::Port()
{
    return GPIOG;
}

uint32_t bsp::GpioPinPG12::Pin()
{
    return GPIO_PIN_12;
}

std::string bsp::GpioPinPG12::PinName() const
{
    return "PG12";
}
