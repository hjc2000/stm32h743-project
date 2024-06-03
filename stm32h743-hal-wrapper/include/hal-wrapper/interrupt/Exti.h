#pragma once
#include<functional>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>
#include<stdint.h>

namespace hal
{
	/// <summary>
	///		* 所有 GPIO 端口的 pin0 连接到中断管理器的 line0，
	///		  所有 GPIO 端口的 pin1 连接到中断管理器的 line1，
	///		  以此类推。
	/// 
	///		* 例如同样都是 pin0，可能来自 GPIOA，GPIOB,...... 等。所有
	///		  这些端口的 pin0 都通过一个多路选择开关连接到中断管理器的 line0 上，
	///		  这个多路选择开关只能同时选择一条路导通。
	/// </summary>
	class Exti
	{
	private:
		Exti() = default;
		friend void ::HAL_GPIO_EXTI_Callback(uint16_t pin);

		std::function<void()> _on_exti0_interrupt;
		std::function<void()> _on_exti1_interrupt;
		std::function<void()> _on_exti2_interrupt;
		std::function<void()> _on_exti3_interrupt;
		std::function<void()> _on_exti4_interrupt;


	public:
		static Exti &Instance()
		{
			static Exti o;
			return o;
		}

		/// <summary>
		///		清除某个 GPIO 引脚上的中断挂起标志。
		/// </summary>
		/// <param name="line"></param>
		void ClearGpioInterruptPending(uint32_t line)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(line);
		}

		/// <summary>
		///		检查指定的线是否有中断标志正被挂起。
		/// </summary>
		bool HasInterruptFlag(uint32_t line)
		{
			return __HAL_GPIO_EXTI_GET_IT(line);
		}

		/// <summary>
		///		使用一条外部中断线。
		/// </summary>
		/// <param name="callback"></param>
		/// <param name="line"></param>
		void UseLine(std::function<void()> callback, uint32_t line);

		/// <summary>
		///		取消使用一条外部中断线。
		///		* 会先禁用对应的线的中断，然后将回调函数设为 nullptr。
		/// </summary>
		/// <param name="line"></param>
		void UnuseLine(uint32_t line);
	};
}
