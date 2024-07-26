#include "DigitalLed.h"
#include <base/Initializer.h>

using namespace bsp;

#pragma region RedDigitalLed
void bsp::RedDigitalLed::TurnOn()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 0 是打开
	hal::GpioPortB::Instance().DigitalWritePin(hal::GpioPinConfig::PinEnum::Pin1, 0);
}

void bsp::RedDigitalLed::TurnOff()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 1 是关闭。
	hal::GpioPortB::Instance().DigitalWritePin(hal::GpioPinConfig::PinEnum::Pin1, 1);
}

void bsp::RedDigitalLed::Toggle()
{
	hal::GpioPortB::Instance().DigitalTogglePin(hal::GpioPinConfig::PinEnum::Pin1);
}
#pragma endregion

#pragma region GreenDigitalLed
void bsp::GreenDigitalLed::TurnOn()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 0 是打开
	hal::GpioPortB::Instance().DigitalWritePin(hal::GpioPinConfig::PinEnum::Pin0, 0);
}

void bsp::GreenDigitalLed::TurnOff()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 1 是关闭。
	hal::GpioPortB::Instance().DigitalWritePin(hal::GpioPinConfig::PinEnum::Pin0, 1);
}

void bsp::GreenDigitalLed::Toggle()
{
	hal::GpioPortB::Instance().DigitalTogglePin(hal::GpioPinConfig::PinEnum::Pin0);
}
#pragma endregion

static base::Initializer _red_led_initializer{
	[]()
	{
		bsp::RedDigitalLed::Instance();
	}};

static base::Initializer _green_led_initializer{
	[]()
	{
		bsp::GreenDigitalLed::Instance();
	}};