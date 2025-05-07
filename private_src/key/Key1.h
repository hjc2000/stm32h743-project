#pragma once
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/UsageStateManager.h"
#include "key_handle.h"

namespace bsp
{
	/// @brief 位于 PE3 引脚。另一端接地，所以需要上拉使用。
	class Key1 :
		public base::key::key_handle
	{
	private:
		base::UsageStateManager<Key1> _usage_state_manager{};
		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortH, 2};

	public:
		Key1();

		bool KeyIsDown() override;
	};
} // namespace bsp
