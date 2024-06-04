#pragma once
#include<bsp-interface/ISysTick.h>
#include<chrono>
#include<hal.h>
#include<stdint.h>

namespace hal
{
	class SysTickClock :public bsp::ISysTick
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

		SysTickClock::SysTickClockSourceOption ClockSource();

		void SetClockSource(SysTickClock::SysTickClockSourceOption value);

		/// <summary>
		///		获取 systick 的时钟频率。
		///		* 这是最终输入到计数器的频率，后面没有任何分频器。
		/// </summary>
		/// <returns></returns>
		uint32_t Frequency() override;

		/// <summary>
		///		获取 SysTick 的 LOAD 寄存器的 RELOAD 部分的值。
		///		RELOAD 占据 LOAD 寄存器的低 23 位。
		///		
		///		RELOAD 是用来在计数值递减到 0 后，下一个时钟周期装载到计数器中的。
		/// </summary>
		/// <returns></returns>
		uint32_t ReloadValue() override;

		/// <summary>
		///		获取 SysTick 的 VAL 寄存器的 CURRENT 部分的值。
		///		CURRENT 部分占据了寄存器的低 23 位，表示当前计数值。
		/// </summary>
		/// <note>
		///		CURRENT 可读可写。只不过这里的写并不是如你所愿地将数据赋予 CURRENT。
		///		写入 CURRENT 会导致 CURRENT 清零，同时会导致 CTRL 寄存器的
		///		COUNTFLAG 位清零。
		/// </note>
		/// <returns>当前计数值</returns>
		uint32_t CurrentValue() override;
	};
}
