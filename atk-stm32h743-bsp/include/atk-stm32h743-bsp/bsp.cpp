#include"bsp.h"
#include<DigitalLed.h>
#include<hal.h>

extern "C"
{
	#include<delay.h>
	#include<led.h>
	#include<sys.h>
}

using namespace bsp;

IDigitalLed &BSP::RedLed()
{
	return RedDigitalLed::Instance();
}

void BSP::Initialize()
{
	Cache_Enable();
	HAL_Init();
	Stm32_Clock_Init(160, 5, 2, 4);
	delay_init(400);
	LED_Init();
	while (1)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		delay_ms(5000);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		delay_ms(5000);
	}
}
