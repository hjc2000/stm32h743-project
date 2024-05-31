#include"TestSerial.h"
#include<bsp/bsp.h>
#include<memory>

void TestSerial()
{
	BSP::RedDigitalLed().TurnOn();
	BSP::Serial().Begin(115200);
	std::unique_ptr<uint8_t[]> buffer { new uint8_t[128] { 72, 72, 72, } };
	while (true)
	{
		//int32_t have_read = BSP::Serial().Read(buffer.get(), 0, 128);
		BSP::GreenDigitalLed().Toggle();
		BSP::Serial().Write(buffer.get(), 0, 3);
		BSP::Delayer().Delay(std::chrono::seconds { 1 });
	}
}
