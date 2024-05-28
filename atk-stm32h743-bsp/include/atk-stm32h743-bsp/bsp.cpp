#include"bsp.h"
#include<DigitalLed.h>

using namespace bsp;

IDigitalLed &BSP::RedLed()
{
	return RedDigitalLed::Instance();
}
