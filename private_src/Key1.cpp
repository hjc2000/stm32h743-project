#include "Key1.h"
#include "base/define.h"
#include "base/peripheral/gpio/gpio_parameter.h"

bsp::Key1 &bsp::Key1::Instance()
{
	static Key1 o{};
	return o;
}

PREINIT(bsp::Key1::Instance)

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
