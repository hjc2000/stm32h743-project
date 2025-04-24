#include "Key1.h"
#include "base/embedded/gpio/gpio_parameter.h"

bsp::Key1::Key1()
{
	_pin.InitializeAsInputMode(base::gpio::PullMode::PullUp,
							   base::gpio::TriggerEdge::Disable);
}

bool bsp::Key1::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !_pin.ReadPin();
}
