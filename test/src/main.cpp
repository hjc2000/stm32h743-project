#include<atk-stm32h743-bsp/bsp.h>

int main(void)
{
	BSP::Initialize();
	while (true)
	{
		BSP::RedLed().Toggle();
	}
}
