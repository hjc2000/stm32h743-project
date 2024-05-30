#include"TestSerial.h"
#include<bsp/bsp.h>
#include<memory>

void TestSerial()
{
	BSP::RedDigitalLed().TurnOn();
	BSP::Serial().Begin(115200);
	std::unique_ptr<uint8_t[]> buffer{ new uint8_t[128] };
	while (1)
	{
		int32_t have_read = BSP::Serial().Read(buffer.get(), 0, 128);
		BSP::GreenDigitalLed().Toggle();
		BSP::Serial().Write(buffer.get(), 0, have_read);
	}
}
