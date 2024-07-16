#pragma once
#include <bsp-interface/ISysTick.h>
#include <chrono>
#include <hal.h>
#include <stdint.h>

namespace hal
{
	class SysTickClock : public bsp::ISysTick
	{
	private:
		SysTickClock() = default;

	public:
		static SysTickClock &Instance()
		{
			static SysTickClock o;
			return o;
		}

		enum class SysTickClockSourceOption
		{
			HCLK = SYSTICK_CLKSOURCE_HCLK,
			HCLK_DIV8 = SYSTICK_CLKSOURCE_HCLK_DIV8,
		};

		/// @brief 获取 SysTickClock 当前使用的时钟源。
		/// @return
		hal::SysTickClock::SysTickClockSourceOption ClockSource();

		/// @brief 设置 Systic 的时钟源。
		/// @note 最好不要设置，因为 freertos 中会修改 Systic 的 CTRL 寄存器的 bit2 从而设置时钟源。
		/// 并且默认情况下，除非你修改 port.c ，否则 freertos 会配置为使用不分频的系统时钟作为 Systic
		/// 的时钟源，即与 CPU 使用相同的时钟源。而且如果不对应修改 freertos 那边的代码，调用本函数
		/// 的结果会被 freertos 覆盖。
		/// @param value
		void SetClockSource(hal::SysTickClock::SysTickClockSourceOption value);

		/// @brief 获取 Systic 的时钟频率。
		/// @note 这是最终输入到计数器的频率，后面没有任何分频器。
		/// @return
		uint32_t Frequency() override;

		/// @brief 获取 SysTick 的 LOAD 寄存器的 RELOAD 部分的值。
		/// RELOAD 占据 LOAD 寄存器的低 23 位。
		/// @note RELOAD 是用来在计数值递减到 0 后，下一个时钟周期装载到计数器中的。
		/// @note 使用了 freertos 后，重装载值会被 freertos 设置。
		/// @return
		uint32_t ReloadValue() override;

		/// @brief 获取 SysTick 的 VAL 寄存器的 CURRENT 部分的值。
		/// @note CURRENT 部分占据了寄存器的低 23 位，表示当前计数值。
		/// @warning CURRENT 可读可写。只不过这里的写并不是如你所愿地将数据赋予 CURRENT。
		/// 写入 CURRENT 会导致 CURRENT 清零，同时会导致 CTRL 寄存器的 COUNTFLAG 位清零。
		///
		/// @return 当前计数值
		uint32_t CurrentValue() override;
	};
}
