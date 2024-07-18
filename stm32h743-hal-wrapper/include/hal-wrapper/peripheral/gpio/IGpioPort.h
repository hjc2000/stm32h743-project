#pragma once
#include <hal-wrapper/peripheral/gpio/GpioPinConfig.h>

namespace hal
{
	/// @brief GPIO 端口抽象类，有几组 GPIO 端口就派生几个本类。
	class IGpioPort
	{
	public:
		virtual GPIO_TypeDef *HardwareInstance() = 0;

		virtual bool IsClockEnabled() = 0;

		bool IsClockDisabled()
		{
			return !IsClockEnabled();
		}

		/// @brief 使能时钟。
		virtual void EnableClock() = 0;

		/// @brief 禁用时钟。
		virtual void DisableClock() = 0;

		void InitPin(GpioPinConfig &config);
		void DigitalWritePin(GpioPinConfig::PinEnum pin, bool value);
		bool DigitalReadPin(GpioPinConfig::PinEnum pin);
		void DigitalTogglePin(GpioPinConfig::PinEnum pin);
	};
}
