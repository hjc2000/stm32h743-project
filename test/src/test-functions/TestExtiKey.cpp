#include"TestExtiKey.h"
#include<bsp/bsp.h>

void TestExtiKey()
{
	BSP::RedDigitalLed().TurnOn();
	while (1)
	{
		if (BSP::WakeUpKey().IsPressed())
		{
			BSP::RedDigitalLed().Toggle();
			BSP::GreenDigitalLed().Toggle();
			BSP::WakeUpKey().ClearPressedFlag();
		}
	}
}
