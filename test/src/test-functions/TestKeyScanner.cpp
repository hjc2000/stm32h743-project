#include "TestKeyScanner.h"
#include <bsp/bsp.h>

using namespace bsp;

void TestKeyScanner()
{
	while (1)
	{
		DI_KeyScanner().ScanKeys();
		if (DI_KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::Key0)))
		{
			BSP::RedDigitalLed().Toggle();
		}

		if (DI_KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::Key1)))
		{
			BSP::GreenDigitalLed().Toggle();
		}

		if (DI_KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::Key2)))
		{
			BSP::RedDigitalLed().Toggle();
			BSP::GreenDigitalLed().Toggle();
		}

		if (DI_KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::KeyWakeUp)))
		{
			BSP::RedDigitalLed().TurnOn();
			BSP::GreenDigitalLed().TurnOn();
		}
	}
}
