#pragma once
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/UsageStateManager.h"
#include "led_handle.h"

namespace bsp
{
	///
	/// @brief 红色 LED
	///
	///
	class RedDigitalLed :
		public base::led::led_handle
	{
	private:
		base::UsageStateManager<RedDigitalLed> _usage_state_manager{};
		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortB, 1};

	public:
		RedDigitalLed();

		///
		/// @brief 打开 LED.
		///
		///
		virtual void TurnOn() override;

		///
		/// @brief 关闭 LED.
		///
		///
		virtual void TurnOff() override;

		///
		/// @brief 翻转 LED.
		///
		///
		virtual void Toggle() override;

		///
		/// @brief LED 灯的状态。
		///
		/// @return base::led::State
		///
		virtual base::led::State State() const override;
	};

} // namespace bsp
