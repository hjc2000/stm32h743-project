#include<atk-stm32h743-bsp/bsp.h>
#include<hal.h>

int main(void)
{
	while (true)
	{
		BSP::RedLed().Toggle();
	}
}
