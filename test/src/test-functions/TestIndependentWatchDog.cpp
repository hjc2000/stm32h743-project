#include "TestIndependentWatchDog.h"
#include <bsp/bsp.h>

void TestIndependentWatchDog()
{
	BSP::Delayer().Delay(std::chrono::milliseconds{500});
	BSP::RedDigitalLed().TurnOn();
	BSP::IndependentWatchDog().SetWatchDogTimeoutDuration(std::chrono::milliseconds(1000));

	while (true)
	{
		BSP::KeyScanner().ScanKeys();
		if (BSP::KeyScanner().HasKeyDownEvent((uint16_t)KeyIndex::Key0))
		{
			BSP::IndependentWatchDog().Feed();
		}

		if (BSP::KeyScanner().HasKeyDownEvent((uint16_t)KeyIndex::Key1))
		{
			BSP::IndependentWatchDog().Feed();
		}
	}
}
