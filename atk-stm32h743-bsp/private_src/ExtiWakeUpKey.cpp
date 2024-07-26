#include "ExtiWakeUpKey.h"
#include <base/Initializer.h>

using namespace bsp;
using namespace hal;

ExtiWakeUpKey::ExtiWakeUpKey()
{
	// 配置引脚
	Port().EnableClock();

	GpioPinConfig options;
	options.SetPin(Pin());
	options.SetMode(hal::GpioPinConfig::ModeEnum::Interrupt_RisingEdgeTrigger);
	options.SetPull(hal::GpioPinConfig::PullOption::PullDown);
	options.SetSpeed(hal::GpioPinConfig::SpeedOption::High);
	Port().InitPin(options);

	Exti::Instance().UseLine([&]()
							 {
		// 这是在中断函数中，禁止使用 Delayer 进行延时。
		hal::SysTickClock::Instance().Delay(std::chrono::milliseconds { 20 });
		_is_pressed = Port().DigitalReadPin(Pin()); }, Pin());
}

base::Initializer _initializer{
	[]()
	{
		bsp::ExtiWakeUpKey::Instance();
	}};