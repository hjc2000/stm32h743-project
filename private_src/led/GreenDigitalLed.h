#pragma once
#include "base/define.h"
#include "base/peripheral/gpio_parameter.h"
#include "base/peripheral/GpioPin.h"
#include "base/peripheral/IDigitalLed.h"

namespace bsp
{
	///
	/// @brief 绿色 LED
	///
	///
	class GreenDigitalLed :
		public base::led::IDigitalLed
	{
	private:
		GreenDigitalLed();

		base::gpio::GpioPin _pin{base::gpio::PortEnum::PortB, 0};

	public:
		static_function GreenDigitalLed &Instance();

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
