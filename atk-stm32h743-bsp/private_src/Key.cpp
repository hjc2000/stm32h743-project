#include "Key.h"

using namespace bsp;

#pragma region Key0
bsp::Key0::Key0()
{
	Port().EnableClock();
	hal::GpioPinConfig options;
	options._mode = hal::GpioPinMode::Input;
	options._pull_mode = hal::GpioPinPull::PullUp;
	options._speed = hal::GpioPinSpeed::High;
	Port().InitPin(Pin(), options);
}

bool bsp::Key0::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !Port().DigitalReadPin(Pin());
}
#pragma endregion

#pragma region Key1
bsp::Key1::Key1()
{
	Port().EnableClock();
	hal::GpioPinConfig options;
	options._mode = hal::GpioPinMode::Input;
	options._pull_mode = hal::GpioPinPull::PullUp;
	options._speed = hal::GpioPinSpeed::High;
	Port().InitPin(Pin(), options);
}

bool bsp::Key1::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !Port().DigitalReadPin(Pin());
}
#pragma endregion

#pragma region Key2
bsp::Key2::Key2()
{
	Port().EnableClock();
	hal::GpioPinConfig options;
	options._mode = hal::GpioPinMode::Input;
	options._pull_mode = hal::GpioPinPull::PullUp;
	options._speed = hal::GpioPinSpeed::High;
	Port().InitPin(Pin(), options);
}

bool bsp::Key2::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !Port().DigitalReadPin(Pin());
}
#pragma endregion

#pragma region KeyWakeUp
bsp::KeyWakeUp::KeyWakeUp()
{
	Port().EnableClock();
	hal::GpioPinConfig options;
	options._mode = hal::GpioPinMode::Input;
	options._pull_mode = hal::GpioPinPull::PullDown;
	options._speed = hal::GpioPinSpeed::High;
	Port().InitPin(Pin(), options);
}

bool bsp::KeyWakeUp::KeyIsDown()
{
	return Port().DigitalReadPin(Pin());
}
#pragma endregion