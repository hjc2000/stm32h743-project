#include "TestIndependentWatchDog.h"
#include <bsp-interface/di.h>
#include <bsp/bsp.h>

void TestIndependentWatchDog()
{
	DI_Delayer().Delay(std::chrono::milliseconds{500});
	BSP::RedDigitalLed().TurnOn();
	BSP::IndependentWatchDog().SetWatchDogTimeoutDuration(std::chrono::milliseconds(1000));

	while (true)
	{
		DI_KeyScanner().ScanKeys();
		if (DI_KeyScanner().HasKeyDownEvent("key0"))
		{
			BSP::IndependentWatchDog().Feed();
		}

		if (DI_KeyScanner().HasKeyDownEvent("key1"))
		{
			BSP::IndependentWatchDog().Feed();
		}
	}
}
