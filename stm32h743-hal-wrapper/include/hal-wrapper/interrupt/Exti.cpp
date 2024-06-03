#include"Exti.h"
#include<hal-wrapper/interrupt/Interrupt.h>

using namespace hal;

/// <summary>
///		EXTI 中断函数的定义。
///		* 记得在启动文件中删除这些符号的 weak 版本，否则链接器有可能优化掉这里的处理函数，
///		  转而使用空操作的 weak 版本。
/// </summary>
extern "C"
{
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

void Exti::UseLine(std::function<void()> callback, uint32_t line)
{
	switch (line)
	{
	case 0:
		{
			_on_exti0_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI0_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI0_IRQn);
			break;
		}
	case 1:
		{
			_on_exti1_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI1_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI1_IRQn);
			break;
		}
	case 2:
		{
			_on_exti2_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI2_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI2_IRQn);
			break;
		}
	case 3:
		{
			_on_exti3_interrupt = callback;
			hal::Interrupt::SetPriority(IRQn_Type::EXTI3_IRQn, 4, 0);
			hal::Interrupt::EnableIRQ(IRQn_Type::EXTI3_IRQn);
			break;
		}
	case 4:
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

void Exti::UnuseLine(uint32_t line)
{
	switch (line)
	{
	case 0:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI0_IRQn);
			_on_exti0_interrupt = nullptr;
			break;
		}
	case 1:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI1_IRQn);
			_on_exti1_interrupt = nullptr;
			break;
		}
	case 2:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI2_IRQn);
			_on_exti2_interrupt = nullptr;
			break;
		}
	case 3:
		{
			hal::Interrupt::DisableIRQ(IRQn_Type::EXTI3_IRQn);
			_on_exti3_interrupt = nullptr;
			break;
		}
	case 4:
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
