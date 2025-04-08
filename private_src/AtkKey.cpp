#include "AtkKey.h"
#include "base/define.h"

#pragma region Key0

bsp::Key0::Key0()
{
	_pin = DI_GpioPinCollection().Get("PH3");
	_pin->OpenAsInputMode(bsp::IGpioPinPullMode::PullUp, bsp::IGpioPinTriggerEdge::Disable);
}

bool bsp::Key0::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !_pin->ReadPin();
}

#pragma endregion

#pragma region Key1

bsp::Key1::Key1()
{
	_pin = DI_GpioPinCollection().Get("PH2");
	_pin->OpenAsInputMode(bsp::IGpioPinPullMode::PullUp, bsp::IGpioPinTriggerEdge::Disable);
}

bool bsp::Key1::KeyIsDown()
{
	// 被按下是低电平，否则是高电平
	return !_pin->ReadPin();
}

#pragma endregion

PREINIT(bsp::Key0::Instance)

bsp::Key0 &bsp::Key0::Instance()
{
	static Key0 o{};
	return o;
}

PREINIT(bsp::Key1::Instance)

bsp::Key1 &bsp::Key1::Instance()
{
	static Key1 o{};
	return o;
}
