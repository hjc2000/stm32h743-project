#pragma once
#include<GpioPort.h>
#include<bsp-interface/IDigitalLed.h>

namespace bsp
{
	/// <summary>
	///		红色 LED
	/// </summary>
	class RedDigitalLed :public bsp::IDigitalLed
	{
	public:
		RedDigitalLed()
		{
			using namespace bsp;
			GpioPortB::Instance().EnableClock();

			/* 使能时钟后写输出寄存器的操作就有效了。先关闭 LED，然后配置为输出模式，
			* 这样 LED 的初始状态就是关闭的。
			*/
			TurnOff();
			GpioPinInitOptions gpio_pin_options;
			gpio_pin_options._mode = GpioPinMode::Output_PushPull;
			gpio_pin_options._pull_mode = GpioPinPull::PullUp;
			gpio_pin_options._speed = GpioPinSpeed::High;
			GpioPortB::Instance().InitPin(GpioPin::Pin0, gpio_pin_options);
		}

		static RedDigitalLed &Instance()
		{
			static RedDigitalLed led;
			return led;
		}

		void TurnOn() override;
		void TurnOff() override;
		void Toggle() override;
	};

	/// <summary>
	///		绿色 LED
	/// </summary>
	class GreenDigitalLed :public bsp::IDigitalLed
	{
	public:
		GreenDigitalLed()
		{
			using namespace bsp;
			GpioPortB::Instance().EnableClock();

			/* 使能时钟后写输出寄存器的操作就有效了。先关闭 LED，然后配置为输出模式，
			* 这样 LED 的初始状态就是关闭的。
			*/
			TurnOff();
			GpioPinInitOptions gpio_pin_options;
			gpio_pin_options._mode = GpioPinMode::Output_PushPull;
			gpio_pin_options._pull_mode = GpioPinPull::PullUp;
			gpio_pin_options._speed = GpioPinSpeed::High;
			GpioPortB::Instance().InitPin(GpioPin::Pin1, gpio_pin_options);
		}

		static GreenDigitalLed &Instance()
		{
			static GreenDigitalLed led;
			return led;
		}

		void TurnOn() override;
		void TurnOff() override;
		void Toggle() override;
	};
}
