#pragma once
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/UsageStateManager.h"
#include "key_handle.h"

namespace bsp
{
	///
	/// @brief 位于 PC13 引脚。另一端接地，所以需要上拉使用。
	///
	class Key2 :
		public base::key::key_handle
	{
	private:
		base::UsageStateManager<Key2> _usage_state_manager{};
		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortC, 13};

	public:
		Key2();

		bool KeyIsDown() override;
	};
} // namespace bsp
