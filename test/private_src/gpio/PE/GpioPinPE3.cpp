#include "GpioPinPE3.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPE3::Port()
{
    return GPIOE;
}

uint32_t bsp::GpioPinPE3::Pin()
{
    return GPIO_PIN_3;
}

std::string bsp::GpioPinPE3::PinName() const
{
    return "PE3";
}
