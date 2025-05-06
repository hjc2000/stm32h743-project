#include "Key2.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/systick/systick.h"
#include "base/unit/Nanoseconds.h"
#include <chrono>

bsp::Key2::Key2()
{
	_pin.InitializeAsInputMode(base::gpio::PullMode::PullUp,
							   base::gpio::TriggerEdge::FallingEdge);

	_pin.RegisterInterruptCallback([this]()
								   {
									   _pressed = true;
									   _last_check_time = base::systick::system_time_stamp();
								   });
}

bool bsp::Key2::KeyIsDown()
{
	if (_pressed)
	{
		base::Nanoseconds current_time = base::systick::system_time_stamp();
		if (current_time - _last_check_time > base::Nanoseconds{std::chrono::milliseconds{20}})
		{
			_pressed = false;
		}
	}

	return _pressed;
}
