#include"TestKeyScanner.h"
#include<bsp/bsp.h>

using namespace bsp;

void TestKeyScanner()
{
	while (1)
	{
		BSP::KeyScanner().ScanKeys();
		if (BSP::KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::Key0)))
		{
			BSP::RedDigitalLed().Toggle();
		}

		if (BSP::KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::Key1)))
		{
			BSP::GreenDigitalLed().Toggle();
		}

		if (BSP::KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::Key2)))
		{
			BSP::RedDigitalLed().Toggle();
			BSP::GreenDigitalLed().Toggle();
		}
	}
}
