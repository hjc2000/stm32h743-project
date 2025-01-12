#pragma once
#include <base/define.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/IDigitalLed.h>
#include <bsp-interface/TaskSingletonGetter.h>

namespace bsp
{
	/// @brief 红色 LED
	class RedDigitalLed :
		public bsp::IDigitalLed
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

		void TurnOn() override;
		void TurnOff() override;
		void Toggle() override;
	};

	/// @brief 绿色 LED
	class GreenDigitalLed : public bsp::IDigitalLed
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

		void TurnOn() override;
		void TurnOff() override;
		void Toggle() override;
	};
} // namespace bsp
