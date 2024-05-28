#pragma once
#include<Exti.h>
#include<GpioPort.h>
#include<Interrupt.h>
#include<Systic.h>
#include<atomic>
#include<bsp-interface/IEventDrivenKey.h>

namespace bsp
{
	/// <summary>
	///		外部中断驱动的按钮，按钮的丝印是 wake up。
	/// </summary>
	class ExtiWakeUpKey :public bsp::IEventDrivenKey
	{
	private:
		/// <summary>
		///		中断函数中经过消抖，确认按键被按下后就会将此字段赋值为 true。
		/// </summary>
		std::atomic_bool _is_pressed = false;

		bsp::IGpioPort &Port()
		{
			return bsp::GpioPortA::Instance();
		}

		bsp::GpioPin Pin()
		{
			return bsp::GpioPin::Pin0;
		}

	public:
		ExtiWakeUpKey();

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
