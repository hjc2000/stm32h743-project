#pragma once
#include<hal.h>

namespace bsp
{
	enum class WindowWatchDogPrescaler
	{
		Div1 = WWDG_PRESCALER_1,
		Div2 = WWDG_PRESCALER_2,
		Div4 = WWDG_PRESCALER_4,
		Div8 = WWDG_PRESCALER_8,
	};

	enum class WindowWatchDogEarlyWakeupInterruptMode
	{
		Enable = WWDG_EWI_ENABLE,
		Disable = WWDG_EWI_DISABLE,
	};

	/// <summary>
	///		窗口看门狗初始化选项。
	/// </summary>
	class WindowWatchDogInitOptions
	{
	public:
		WindowWatchDogInitOptions() = default;
		WindowWatchDogInitOptions(WindowWatchDogInitOptions const &o) = default;
		WindowWatchDogInitOptions(WWDG_InitTypeDef const &o);
		WindowWatchDogInitOptions &operator=(WindowWatchDogInitOptions const &o) = default;
		WindowWatchDogInitOptions &operator=(WWDG_InitTypeDef const &o);

		WindowWatchDogPrescaler _prescaler;

		/// <summary>
		///		窗口上界，范围：[0x40, 0x7F]
		/// </summary>
		uint32_t _window_upper_bound;

		/// <summary>
		///		计数器重载值。
		///		* 范围：[0x40, 0x7F]
		///		* 一般要大于 _window_upper_bound。如果小于 _window_upper_bound，
		///		  那实际的喂狗窗口时间段就会比 _window_upper_bound - 0x40 小，此时
		///		  _window_upper_bound 虽然比 _counter_reload_value 大，但是没有
		///		  意义。
		/// </summary>
		uint32_t _counter_reload_value;

		/// <summary>
		///		选择看门狗即将超时时是否触发 “提早唤醒中断”。
		/// </summary>
		WindowWatchDogEarlyWakeupInterruptMode _early_wakeup_interrupt_mode;

		operator WWDG_InitTypeDef() const;
	};
}