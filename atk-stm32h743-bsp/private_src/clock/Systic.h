#pragma once
#include<chrono>
#include<GpioPort.h>
#include<hal.h>
#include<stdint.h>

namespace bsp
{
	enum class SysticClockSource
	{
		HCLK = SYSTICK_CLKSOURCE_HCLK,
		HCLK_DIV8 = SYSTICK_CLKSOURCE_HCLK_DIV8,
	};

	/// <summary>
	///		Systic 只有 1 个，所以实现为静态类。
	/// </summary>
	class Systic
	{
	private:
		Systic() = delete;
		Systic(Systic const &another) = delete;
		Systic &operator=(Systic const &value) = delete;

	public:
		/// <summary>
		///		获取 SysTick 寄存器组中的 CTRL 寄存器的 COUNTFLAG 位的值。
		///		该位是只读的。
		/// </summary>
		/// 
		/// <note>
		///		SysTick 是一个减计数的计数器。如果自上次读取 SysTick 的值后，SysTick 减计数到 0 了，
		///		说明计数值要发生回绕了，此时 CTRL 寄存器的 COUNTFLAG 位会置位，则本函数返回 1.
		/// </note>
		/// <note>
		///		读取 COUNTFLAG 位会导致 COUNTFLAG 位自动清零。
		/// </note>
		/// 
		/// <returns>发生了回绕 返回非 0 值，没有发生回绕返回 0.</returns>
		static bool CountFlag();

		static SysticClockSource ClockSource();

		/// <summary>
		///		设置完后必须使用 ClockSignal::Config 配置一下 ClockType::SYSCLK，
		///		否则仅仅是更新标志位，不会实际配置 sysclk，这会导致状态不同步。
		/// </summary>
		/// <param name="value"></param>
		static void SetClockSource(SysticClockSource value);

		/// <summary>
		///		获取 systick 的时钟频率。
		///		* 这是最终输入到计数器的频率，后面没有任何分频器。
		/// </summary>
		/// <returns></returns>
		static uint32_t Frequency();

		/// <summary>
		///		获取 SysTick 的 LOAD 寄存器的 RELOAD 部分的值。
		///		RELOAD 占据 LOAD 寄存器的低 23 位。
		///		
		///		RELOAD 是用来在计数值递减到 0 后，下一个时钟周期装载到计数器中的。
		/// </summary>
		/// <returns></returns>
		static uint32_t ReloadNum();

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
		static uint32_t CurrentValue();

		/// <summary>
		///		通过空指令循环来延时
		/// </summary>
		/// <param name="tick_count">要延时的 SysTick 计数值</param>
		static void NopLoopDelayForTicks(uint32_t tick_count);
		static void NopLoopDelay(std::chrono::microseconds microseconds);
		static void NopLoopDelay(std::chrono::milliseconds milliseconds);
		static void NopLoopDelay(std::chrono::seconds seconds);
	};
}
