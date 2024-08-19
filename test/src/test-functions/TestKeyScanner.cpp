#include "TestKeyScanner.h"
#include <bsp/bsp.h>

using namespace bsp;

void TestKeyScanner()
{
	while (1)
	{
		DI_KeyScanner().ScanKeys();
		if (DI_KeyScanner().HasKeyDownEvent("key0"))
		{
			BSP::RedDigitalLed().Toggle();
		}

		if (DI_KeyScanner().HasKeyDownEvent("key1"))
		{
			BSP::GreenDigitalLed().Toggle();
		}

		if (DI_KeyScanner().HasKeyDownEvent("key2"))
		{
			BSP::RedDigitalLed().Toggle();
			BSP::GreenDigitalLed().Toggle();
		}

		if (DI_KeyScanner().HasKeyDownEvent("key_wake_up"))
		{
			BSP::RedDigitalLed().TurnOn();
			BSP::GreenDigitalLed().TurnOn();
		}
	}
}
