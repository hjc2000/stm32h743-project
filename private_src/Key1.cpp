#include "Key1.h"
#include "base/peripheral/gpio/gpio_parameter.h"
#include "base/UsageStateManager.h"

namespace
{
	base::UsageStateManager _usage_state_manager;
}

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
