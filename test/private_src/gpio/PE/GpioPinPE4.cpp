#include "GpioPinPE4.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPE4::Port()
{
    return GPIOE;
}

uint32_t bsp::GpioPinPE4::Pin()
{
    return GPIO_PIN_4;
}
