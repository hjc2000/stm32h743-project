#pragma once
#include<hal.h>

namespace bsp
{
	/// <summary>
	///		基本定时器初始化选项
	/// </summary>
	class BaseTimerInitOptions
	{
	public:
		BaseTimerInitOptions() = default;
		BaseTimerInitOptions(TIM_Base_InitTypeDef const &o);
		BaseTimerInitOptions &operator=(TIM_Base_InitTypeDef const &o);
		operator TIM_Base_InitTypeDef() const;

		/// <summary>
		///		预分频器。范围：[0x0000, 0xFFFF]
		/// </summary>
		uint32_t _prescaler;

		enum class CounterMode
		{
			Up = TIM_COUNTERMODE_UP,
			Down = TIM_COUNTERMODE_DOWN,
			CenterAligned1 = TIM_COUNTERMODE_CENTERALIGNED1,
			CenterAligned2 = TIM_COUNTERMODE_CENTERALIGNED2,
			CenterAligned3 = TIM_COUNTERMODE_CENTERALIGNED3,
		};

		CounterMode _counter_mode;

		/// <summary>
		///		范围：[0x0000, 0xFFFF]
		/// </summary>
		uint32_t _period;

		/// <summary>
		///		自动重载预装载使能。
		///		* 禁用时，写入预装载寄存器的值会直接送到影子寄存器中，不经过缓冲。
		/// </summary>
		bool _is_auto_reload_preload_enabled;
	};
}
