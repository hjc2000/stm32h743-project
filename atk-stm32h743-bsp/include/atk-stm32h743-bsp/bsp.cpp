#include"bsp.h"
#include<Cache.h>
#include<DigitalLed.h>
#include<hal.h>

extern "C"
{
	#include<delay.h>
	#include<sys.h>
}

using namespace bsp;

IDigitalLed &BSP::RedDigitalLed()
{
	return RedDigitalLed::Instance();
}

IDigitalLed &BSP::GreenDigitalLed()
{
	return GreenDigitalLed::Instance();
}

void BSP::Initialize()
{
	Cache::Enable();
	HAL_Init();
	Stm32_Clock_Init(160, 5, 2, 4);
	delay_init(400);
}
