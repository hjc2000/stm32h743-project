#include "AtkKey.h"

using namespace bsp;

#pragma region Key0
bsp::Key0::Key0()
{
	using namespace bsp;
	Port().EnableClock();
	GpioPinInitOptions options;
	options._mode = GpioPinMode::Input;
	options._pull_mode = GpioPinPull::PullUp;
	options._speed = GpioPinSpeed::High;
	Port().InitPin(GpioPin::Pin4, options);
}

bool bsp::Key0::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !Port().DigitalReadPin(GpioPin::Pin4);
}
#pragma endregion

#pragma region Key1
bsp::Key1::Key1()
{
	using namespace bsp;
	Port().EnableClock();
	GpioPinInitOptions options;
	options._mode = GpioPinMode::Input;
	options._pull_mode = GpioPinPull::PullUp;
	options._speed = GpioPinSpeed::High;
	Port().InitPin(GpioPin::Pin3, options);
}

bool bsp::Key1::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !Port().DigitalReadPin(GpioPin::Pin3);
}
#pragma endregion
