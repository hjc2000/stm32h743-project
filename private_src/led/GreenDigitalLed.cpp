#include "GreenDigitalLed.h"

bsp::GreenDigitalLed::GreenDigitalLed()
{
	_pin.InitializeAsOutputMode(base::gpio::PullMode::PullUp,
								base::gpio::DriveMode::PushPull);

	TurnOff();
}

void bsp::GreenDigitalLed::TurnOn()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 0 是打开
	_pin.WritePin(0);
}

void bsp::GreenDigitalLed::TurnOff()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 1 是关闭。
	_pin.WritePin(1);
}

void bsp::GreenDigitalLed::Toggle()
{
	_pin.TogglePin();
}

base::led::State bsp::GreenDigitalLed::State() const
{
	if (!_pin.ReadPin())
	{
		return base::led::State::On;
	}

	return base::led::State::Off;
}
