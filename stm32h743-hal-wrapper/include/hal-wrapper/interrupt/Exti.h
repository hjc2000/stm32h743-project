#pragma once
#include<functional>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>

extern "C"
{
	void HAL_GPIO_EXTI_Callback(uint16_t pin);
}

namespace hal
{
	/// <summary>
	///		* 所有 GPIO 端口的 pin0 连接到中断管理器的 line0，
	///		  所有 GPIO 端口的 pin1 连接到中断管理器的 line1，
	///		  以此类推。
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
		///		使用一条外部中断线。
		/// </summary>
		/// <param name="callback"></param>
		/// <param name="pin"></param>
		void UseLine(std::function<void()> callback, hal::GpioPin pin);

		/// <summary>
		///		取消使用一条外部中断线。
		///		* 会先禁用对应的线的中断，然后将回调函数设为 nullptr。
		/// </summary>
		/// <param name="pin"></param>
		void UnuseLine(hal::GpioPin pin);
	};
}
