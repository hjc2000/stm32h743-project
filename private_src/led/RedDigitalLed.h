#pragma once
#include "base/define.h"
#include "base/peripheral/gpio/gpio_parameter.h"
#include "base/peripheral/gpio/GpioPin.h"
#include "base/peripheral/led/IDigitalLed.h"
#include "led_handle.h"

namespace bsp
{
	///
	/// @brief 红色 LED
	///
	///
	class RedDigitalLed :
		public base::led::IDigitalLed,
		public base::led::led_handle
	{
	private:
		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortB, 1};

	public:
		RedDigitalLed();

		static_function RedDigitalLed &Instance();

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
