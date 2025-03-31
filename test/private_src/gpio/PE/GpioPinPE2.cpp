#include "GpioPinPE2.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPE2::Port()
{
    return GPIOE;
}

uint32_t bsp::GpioPinPE2::Pin()
{
    return GPIO_PIN_2;
}

std::string bsp::GpioPinPE2::PinName() const
{
    return "PE2";
}
