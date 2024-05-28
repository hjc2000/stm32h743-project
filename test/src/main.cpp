#include<atk-stm32h743-bsp/bsp.h>
#include<atomic>
#include<stdexcept>
#include<stdint.h>

volatile double count = 0;

int main(void)
{
	while (true)
	{
		try
		{

			BSP::Initialize();
			while (true)
			{
				BSP::RedDigitalLed().TurnOn();
				BSP::GreenDigitalLed().TurnOn();
			}
		}
		catch (std::exception &e)
		{

		}
	}
}
