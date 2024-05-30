#include"Exti.h"
#include<hal-wrapper/interrupt/Interrupt.h>

using namespace hal;

/// <summary>
///		EXTI 中断函数的定义。
///		* 记得在启动文件中删除这些符号的 weak 版本。
/// </summary>
extern "C"
{
	/// <summary>
	///		这里直接模仿了 HAL 库的 HAL_GPIO_EXTI_IRQHandler 函数。注意，更新 HAL 库
	///		后要记得查看 HAL_GPIO_EXTI_IRQHandler 函数是否变动。
	/// </summary>
	void EXTI0_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)GpioPin::Pin0);
	}

	void EXTI1_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)GpioPin::Pin1);
	}

	void EXTI2_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)GpioPin::Pin2);
	}

	void EXTI3_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)GpioPin::Pin3);
	}

	void EXTI4_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)GpioPin::Pin4);
	}

	/// <summary>
	///		重写 HAL 库中的 weak 版本
	/// </summary>
	/// <param name="GPIO_Pin"></param>
	void HAL_GPIO_EXTI_Callback(uint16_t pin)
	{
		switch (pin)
		{
		case (uint16_t)GpioPin::Pin0:
			{
				if (Exti::Instance()._on_exti0_interrupt)
				{
					Exti::Instance()._on_exti0_interrupt();
				}

				break;
			}
		case (uint16_t)GpioPin::Pin1:
			{
				if (Exti::Instance()._on_exti1_interrupt)
				{
					Exti::Instance()._on_exti1_interrupt();
				}

				break;
			}
		case (uint16_t)GpioPin::Pin2:
			{
				if (Exti::Instance()._on_exti2_interrupt)
				{
					Exti::Instance()._on_exti2_interrupt();
				}

				break;
			}
		case (uint16_t)GpioPin::Pin3:
			{
				if (Exti::Instance()._on_exti3_interrupt)
				{
					Exti::Instance()._on_exti3_interrupt();
				}

				break;
			}
		case (uint16_t)GpioPin::Pin4:
			{
				if (Exti::Instance()._on_exti4_interrupt)
				{
					Exti::Instance()._on_exti4_interrupt();
				}

				break;
			}
		default:
			{
				break;
			}
		}
	}
}// extern "C"

void Exti::UseLine(std::function<void()> callback, GpioPin pin)
{
	switch (pin)
	{
	case GpioPin::Pin0:
		{
			_on_exti0_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI0_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI0_IRQn);
			break;
		}
	case GpioPin::Pin1:
		{
			_on_exti1_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI1_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI1_IRQn);
			break;
		}
	case GpioPin::Pin2:
		{
			_on_exti2_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI2_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI2_IRQn);
			break;
		}
	case GpioPin::Pin3:
		{
			_on_exti3_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI3_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI3_IRQn);
			break;
		}
	case GpioPin::Pin4:
		{
			_on_exti4_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI4_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI4_IRQn);
			break;
		}
	default:
		{
			break;
		}
	}
}

void Exti::UnuseLine(GpioPin pin)
{
	switch (pin)
	{
	case GpioPin::Pin0:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI0_IRQn);
			_on_exti0_interrupt = nullptr;
			break;
		}
	case GpioPin::Pin1:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI1_IRQn);
			_on_exti1_interrupt = nullptr;
			break;
		}
	case GpioPin::Pin2:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI2_IRQn);
			_on_exti2_interrupt = nullptr;
			break;
		}
	case GpioPin::Pin3:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI3_IRQn);
			_on_exti3_interrupt = nullptr;
			break;
		}
	case GpioPin::Pin4:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI4_IRQn);
			_on_exti4_interrupt = nullptr;
			break;
		}
	default:
		{
			break;
		}
	}
}
