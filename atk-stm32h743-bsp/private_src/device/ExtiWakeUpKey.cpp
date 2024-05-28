#include"ExtiWakeUpKey.h"

using namespace bsp;
using namespace bsp;

ExtiWakeUpKey::ExtiWakeUpKey()
{
	// 配置引脚
	Port().EnableClock();
	GpioPinInitOptions options;
	options._mode = GpioPinMode::Interrupt_RisingEdgeTrigger;
	options._pull_mode = GpioPinPull::PullDown;
	options._speed = GpioPinSpeed::High;
	Port().InitPin(Pin(), options);
	Exti::Instance().UseLine([&]()
	{
		// 这是在中断函数中，禁止使用 Delayer 进行延时。
		Systic::NopLoopDelay(std::chrono::milliseconds(20));
		_is_pressed = Port().DigitalReadPin(Pin());
		Exti::Instance().ClearGpioInterruptPending(Pin());
	}, Pin());
}
