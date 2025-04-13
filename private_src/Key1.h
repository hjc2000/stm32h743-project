#pragma once
#include "base/define.h"
#include "base/peripheral/gpio/gpio_parameter.h"
#include "base/peripheral/gpio/GpioPin.h"
#include "bsp-interface/key/IKey.h"
#include "key_handle.h"

namespace bsp
{
	/// @brief 位于 PE3 引脚。另一端接地，所以需要上拉使用。
	class Key1 :
		public bsp::IKey,
		public base::key::key_handle
	{
	private:
		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortH, 2};

	public:
		Key1();

		static_function Key1 &Instance();

		std::string KeyName() override
		{
			return "key1";
		}

		bool KeyIsDown() override;
	};
} // namespace bsp
