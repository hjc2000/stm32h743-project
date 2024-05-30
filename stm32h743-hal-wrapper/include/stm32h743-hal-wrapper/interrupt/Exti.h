#pragma once
#include<functional>
#include<stdint.h>
#include<stm32h743-hal-wrapper/peripheral/gpio/GpioPort.h>

namespace hal
{
	/// <summary>
	///		本类不提供开启中断的功能。设置好回调函数后，需要用 Interrupt 类来开启
	///		EXTI 中断。
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
		///		* 所有 GPIO 端口的 pin0 连接到中断管理器的 line0，
		///		  所有 GPIO 端口的 pin1 连接到中断管理器的 line1，
		///		  以此类推。
		///		* 例如同样都是 pin0，可能来自 GPIOA，GPIOB,...... 等。所有
		///		  这些端口的 pin0 都通过一个多路选择开关连接到中断管理器的 line0 上，
		///		  这个多路选择开关只能同时选择一条路导通。
		///		* 这里的清除某个引脚上的中断挂起标志其实应该说是清除某个 line 上的中断挂起标志。
		///		  只不过这些 line 刚好跟 GpioPin 枚举值一一对应而已。
		///		* 所谓中断挂起标志就是表示发生了中断，但是中断服务程序还没被执行。可能是因为禁用了
		///		  中断。
		/// </summary>
		/// <param name="pin"></param>
		void ClearGpioInterruptPending(GpioPin pin)
		{
			__HAL_GPIO_EXTI_CLEAR_IT((uint32_t)pin);
		}

		/// <summary>
		///		检查指定的线是否有中断标志正被挂起。
		/// </summary>
		bool HasInterruptFlag(GpioPin pin)
		{
			return __HAL_GPIO_EXTI_GET_IT((uint32_t)pin);
		}

		/// <summary>
		///		使用一条外部中断线。
		/// </summary>
		/// <param name="callback"></param>
		/// <param name="pin"></param>
		void UseLine(std::function<void()> callback, GpioPin pin);

		/// <summary>
		///		取消使用一条外部中断线。
		///		* 会先禁用对应的线的中断，然后将回调函数设为 nullptr。
		/// </summary>
		/// <param name="pin"></param>
		void UnuseLine(GpioPin pin);
	};
}
