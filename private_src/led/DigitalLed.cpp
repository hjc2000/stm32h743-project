#include "DigitalLed.h"
#include "base/define.h"
#include "base/peripheral/IDigitalLed.h"

using namespace bsp;

/* #region 红色 LED */

bsp::RedDigitalLed::RedDigitalLed()
{
	_pin = DI_GpioPinCollection().Get("PB1");

	_pin->OpenAsOutputMode(bsp::IGpioPinPullMode::PullUp,
						   bsp::IGpioPinDriver::PushPull);

	TurnOff();
}

PREINIT(bsp::RedDigitalLed::Instance)

bsp::RedDigitalLed &bsp::RedDigitalLed::Instance()
{
	static RedDigitalLed o{};
	return o;
}

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

base::led::State bsp::RedDigitalLed::State() const
{
	if (!_pin->ReadPin())
	{
		return base::led::State::On;
	}

	return base::led::State::Off;
}

/* #endregion */
