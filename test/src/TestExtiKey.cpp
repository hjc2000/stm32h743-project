#include "TestExtiKey.h"
#include <bsp-interface/di/led.h>
#include <ExtiWakeUpKey.h>

void TestExtiKey()
{
    DI_RedDigitalLed().TurnOn();
    while (1)
    {
        if (bsp::ExtiWakeUpKey::Instance().IsPressed())
        {
            DI_RedDigitalLed().Toggle();
            DI_GreenDigitalLed().Toggle();
            bsp::ExtiWakeUpKey::Instance().ClearPressedFlag();
        }
    }
}
