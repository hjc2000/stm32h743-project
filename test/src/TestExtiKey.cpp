#include "TestExtiKey.h"
#include <bsp-interface/di/led.h>
#include <ExtiWakeUpKey.h>

void TestExtiKey()
{
	bsp::di::led::RedDigitalLed().TurnOn();
	while (1)
	{
		if (bsp::ExtiWakeUpKey::Instance().IsPressed())
		{
			bsp::di::led::RedDigitalLed().Toggle();
			bsp::di::led::GreenDigitalLed().Toggle();
			bsp::ExtiWakeUpKey::Instance().ClearPressedFlag();
		}
	}
}
