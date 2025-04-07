#include "TestExtiKey.h"
#include "base/peripheral/IDigitalLed.h"
#include <ExtiWakeUpKey.h>

void TestExtiKey()
{
	base::led::RedDigitalLed().TurnOn();
	while (1)
	{
		if (bsp::ExtiWakeUpKey::Instance().IsPressed())
		{
			base::led::RedDigitalLed().Toggle();
			base::led::GreenDigitalLed().Toggle();
			bsp::ExtiWakeUpKey::Instance().ClearPressedFlag();
		}
	}
}
