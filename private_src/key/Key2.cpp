#include "Key2.h"
#include "base/embedded/gpio/gpio_parameter.h"

bsp::Key2::Key2()
{
	_pin.InitializeAsInputMode(base::gpio::PullMode::PullUp,
							   base::gpio::TriggerEdge::Disable);
}

bool bsp::Key2::KeyIsDown()
{
	return !_pin.ReadPin();
}
