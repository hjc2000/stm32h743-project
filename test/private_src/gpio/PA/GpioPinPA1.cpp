#include "GpioPinPA1.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPA1::Port()
{
    return GPIOA;
}

uint32_t bsp::GpioPinPA1::Pin()
{
    return GPIO_PIN_1;
}

std::string bsp::GpioPinPA1::PinName() const
{
    return "PA1";
}
