#pragma once
#include "base/define.h"
#include "base/peripheral/gpio/GpioPin.h"
#include "bsp-interface/key/IKey.h"
#include "key_handle.h"

namespace bsp
{
	/// @brief 位于 PE4 引脚上的按键。另一端接地，所以要上拉使用。
	class Key0 :
		public bsp::IKey,
		public base::key::key_handle
	{
	private:
		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortH, 3};

	public:
		Key0();

		static_function Key0 &Instance();

		std::string KeyName() override
		{
			return "key0";
		}

		bool KeyIsDown() override;
	};
} // namespace bsp
