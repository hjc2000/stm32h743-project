#pragma once
#include<atomic>
#include<bsp-interface/IEventDrivenKey.h>
#include<hal-wrapper/clock/SysTickClock.h>
#include<hal-wrapper/interrupt/Exti.h>
#include<hal-wrapper/interrupt/Interrupt.h>

namespace bsp
{
	/// <summary>
	///		外部中断驱动的按钮，按钮的丝印是 wake up。
	/// </summary>
	class ExtiWakeUpKey :public bsp::IEventDrivenKey
	{
	private:
		ExtiWakeUpKey();

		/// <summary>
		///		中断函数中经过消抖，确认按键被按下后就会将此字段赋值为 true。
		/// </summary>
		std::atomic_bool _is_pressed = false;

		hal::IGpioPort &Port()
		{
			return hal::GpioPortA::Instance();
		}

		hal::GpioPinConfig::PinEnum Pin()
		{
			return hal::GpioPinConfig::PinEnum::Pin0;
		}

	public:
		static ExtiWakeUpKey &Instance()
		{
			static ExtiWakeUpKey instance;
			return instance;
		}

		bool IsPressed() override
		{
			return _is_pressed;
		}

		void ClearPressedFlag() override
		{
			_is_pressed = false;
		}
	};
}
