#include"TestKeyScanner.h"
#include<atk-stm32h743-bsp/bsp.h>

using namespace bsp;

void TestKeyScanner()
{
	while (1)
	{
		BSP::KeyScanner().ScanKeys();
		if (BSP::KeyScanner().HasKeyDownEvent((uint16_t)KeyIndex::Key0))
		{
			BSP::RedDigitalLed().Toggle();
		}

		if (BSP::KeyScanner().HasKeyDownEvent((uint16_t)KeyIndex::Key1))
		{
			BSP::GreenDigitalLed().Toggle();
		}
	}
}
