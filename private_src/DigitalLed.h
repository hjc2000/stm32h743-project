#pragma once
#include "base/peripheral/IDigitalLed.h"
#include <base/define.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>

namespace bsp
{
	/// @brief 红色 LED
	class RedDigitalLed :
		public base::led::IDigitalLed
	{
	private:
		RedDigitalLed()
		{
			_pin = DI_GpioPinCollection().Get("PB1");
			_pin->OpenAsOutputMode(bsp::IGpioPinPullMode::PullUp, bsp::IGpioPinDriver::PushPull);
			TurnOff();
		}

		bsp::IGpioPin *_pin = nullptr;

	public:
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
		virtual base::led::State State() override;
	};

	/// @brief 绿色 LED
	class GreenDigitalLed :
		public base::led::IDigitalLed
	{
	private:
		GreenDigitalLed()
		{
			_pin = DI_GpioPinCollection().Get("PB0");
			_pin->OpenAsOutputMode(bsp::IGpioPinPullMode::PullUp, bsp::IGpioPinDriver::PushPull);
			TurnOff();
		}

		bsp::IGpioPin *_pin = nullptr;

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
		virtual base::led::State State() override;
	};
} // namespace bsp
