#pragma once
#include "base/peripheral/IDigitalLed.h"
#include <base/define.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>

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
		static_function RedDigitalLed &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<RedDigitalLed>
			{
			public:
				std::unique_ptr<RedDigitalLed> Create() override
				{
					return std::unique_ptr<RedDigitalLed>{new RedDigitalLed{}};
				}
			};

			Getter g;
			return g.Instance();
		}

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
		static_function GreenDigitalLed &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GreenDigitalLed>
			{
			public:
				std::unique_ptr<GreenDigitalLed> Create() override
				{
					return std::unique_ptr<GreenDigitalLed>{new GreenDigitalLed{}};
				}
			};

			Getter g;
			return g.Instance();
		}

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
