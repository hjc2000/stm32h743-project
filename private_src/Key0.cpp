#include "Key0.h"
#include "base/UsageStateManager.h"

namespace
{
	base::UsageStateManager _usage_state_manager;
}

bsp::Key0::Key0()
{
	_pin.InitializeAsInputMode(base::gpio::PullMode::PullUp,
							   base::gpio::TriggerEdge::Disable);
}

bool bsp::Key0::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !_pin.ReadPin();
}
