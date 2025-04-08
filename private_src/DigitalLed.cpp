#include "DigitalLed.h"
#include "base/define.h"
#include "base/peripheral/IDigitalLed.h"

using namespace bsp;

#pragma region RedDigitalLed

void bsp::RedDigitalLed::TurnOn()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 0 是打开
	_pin->WritePin(0);
}

void bsp::RedDigitalLed::TurnOff()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 1 是关闭。
	_pin->WritePin(1);
}

void bsp::RedDigitalLed::Toggle()
{
	_pin->TogglePin();
}

#pragma endregion

#pragma region GreenDigitalLed

void bsp::GreenDigitalLed::TurnOn()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 0 是打开
	_pin->WritePin(0);
}

void bsp::GreenDigitalLed::TurnOff()
{
	// GPIO 引脚连接着 LED 的阴极，所以写 1 是关闭。
	_pin->WritePin(1);
}

void bsp::GreenDigitalLed::Toggle()
{
	_pin->TogglePin();
}

#pragma endregion

base::led::State bsp::RedDigitalLed::State()
{
	if (!_pin->ReadPin())
	{
		return base::led::State::On;
	}

	return base::led::State::Off;
}

base::led::State bsp::GreenDigitalLed::State()
{
	if (!_pin->ReadPin())
	{
		return base::led::State::On;
	}

	return base::led::State::Off;
}

PREINIT(bsp::RedDigitalLed::Instance)

bsp::RedDigitalLed &bsp::RedDigitalLed::Instance()
{
	static RedDigitalLed o{};
	return o;
}

PREINIT(bsp::GreenDigitalLed::Instance)

bsp::GreenDigitalLed &bsp::GreenDigitalLed::Instance()
{
	static GreenDigitalLed o;
	return o;
}
