#include "Key0.h"

bsp::Key0 &bsp::Key0::Instance()
{
	static Key0 o{};
	return o;
}

PREINIT(bsp::Key0::Instance)

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
