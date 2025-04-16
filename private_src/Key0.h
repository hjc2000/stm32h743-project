#pragma once
#include "base/peripheral/gpio/GpioPin.h"
#include "base/UsageStateManager.h"
#include "key_handle.h"

namespace bsp
{
	/// @brief 位于 PE4 引脚上的按键。另一端接地，所以要上拉使用。
	class Key0 :
		public base::key::key_handle
	{
	private:
		base::UsageStateManager<Key0> _usage_state_manager{};
		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortH, 3};

	public:
		Key0();

		bool KeyIsDown() override;
	};
} // namespace bsp
