#include"DigitalLed.h"

using namespace bsp;
using namespace bsp;

#pragma region RedDigitalLed
void bsp::RedDigitalLed::TurnOn()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 0 是打开
	GpioPortB::Instance().DigitalWritePin(GpioPin::Pin5, 0);
}

void bsp::RedDigitalLed::TurnOff()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 1 是关闭。
	GpioPortB::Instance().DigitalWritePin(GpioPin::Pin5, 1);
}

void bsp::RedDigitalLed::Toggle()
{
	GpioPortB::Instance().DigitalTogglePin(GpioPin::Pin5);
}
#pragma endregion

#pragma region GreenDigitalLed
void bsp::GreenDigitalLed::TurnOn()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 0 是打开
	GpioPortE::Instance().DigitalWritePin(GpioPin::Pin5, 0);
}

void bsp::GreenDigitalLed::TurnOff()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 1 是关闭。
	GpioPortE::Instance().DigitalWritePin(GpioPin::Pin5, 1);
}

void bsp::GreenDigitalLed::Toggle()
{
	GpioPortE::Instance().DigitalTogglePin(GpioPin::Pin5);
}
#pragma endregion
