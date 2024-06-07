#include"Exti.h"
#include<hal-wrapper/interrupt/Interrupt.h>
#include<stdexcept>

using namespace hal;

/// <summary>
///		EXTI 中断函数的定义。
///		* 记得在启动文件中删除这些符号的 weak 版本。
/// </summary>
extern "C"
{
	void EXTI0_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)hal::GpioPinConfig::PinEnum::Pin0);
	}

	void EXTI1_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)hal::GpioPinConfig::PinEnum::Pin1);
	}

	void EXTI2_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)hal::GpioPinConfig::PinEnum::Pin2);
	}

	void EXTI3_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)hal::GpioPinConfig::PinEnum::Pin3);
	}

	void EXTI4_IRQHandler()
	{
		HAL_GPIO_EXTI_IRQHandler((uint16_t)hal::GpioPinConfig::PinEnum::Pin4);
	}

	/// <summary>
	///		重写 HAL 库中的 weak 版本
	/// </summary>
	/// <param name="pin"></param>
	void HAL_GPIO_EXTI_Callback(uint16_t pin)
	{
		// 这里传进来的是 hal::GpioPinConfig::PinEnum 类型，需要手动强制转换一下。
		switch (static_cast<hal::GpioPinConfig::PinEnum>(pin))
		{
		case hal::GpioPinConfig::PinEnum::Pin0:
			{
				if (Exti::Instance()._on_exti0_interrupt)
				{
					Exti::Instance()._on_exti0_interrupt();
				}

				break;
			}
		case hal::GpioPinConfig::PinEnum::Pin1:
			{
				if (Exti::Instance()._on_exti1_interrupt)
				{
					Exti::Instance()._on_exti1_interrupt();
				}

				break;
			}
		case hal::GpioPinConfig::PinEnum::Pin2:
			{
				if (Exti::Instance()._on_exti2_interrupt)
				{
					Exti::Instance()._on_exti2_interrupt();
				}

				break;
			}
		case hal::GpioPinConfig::PinEnum::Pin3:
			{
				if (Exti::Instance()._on_exti3_interrupt)
				{
					Exti::Instance()._on_exti3_interrupt();
				}

				break;
			}
		case hal::GpioPinConfig::PinEnum::Pin4:
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

void Exti::UseLine(std::function<void()> callback, hal::GpioPinConfig::PinEnum pin)
{
	switch (pin)
	{
	case hal::GpioPinConfig::PinEnum::Pin0:
		{
			_on_exti0_interrupt = callback;
			Interrupt::SetPriority(IRQn_Type::EXTI0_IRQn, 4, 0);
			Interrupt::EnableIRQ(IRQn_Type::EXTI0_IRQn);
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin1:
		{
			_on_exti1_interrupt = callback;
			Interrupt::SetPriority(IRQn_Type::EXTI1_IRQn, 4, 0);
			Interrupt::EnableIRQ(IRQn_Type::EXTI1_IRQn);
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin2:
		{
			_on_exti2_interrupt = callback;
			Interrupt::SetPriority(IRQn_Type::EXTI2_IRQn, 4, 0);
			Interrupt::EnableIRQ(IRQn_Type::EXTI2_IRQn);
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin3:
		{
			_on_exti3_interrupt = callback;
			Interrupt::SetPriority(IRQn_Type::EXTI3_IRQn, 4, 0);
			Interrupt::EnableIRQ(IRQn_Type::EXTI3_IRQn);
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin4:
		{
			_on_exti4_interrupt = callback;
			Interrupt::SetPriority(IRQn_Type::EXTI4_IRQn, 4, 0);
			Interrupt::EnableIRQ(IRQn_Type::EXTI4_IRQn);
			break;
		}
	default:
		{
			throw std::invalid_argument { "pin 超出范围。" };
		}
	}
}

void Exti::UnuseLine(hal::GpioPinConfig::PinEnum pin)
{
	switch (pin)
	{
	case hal::GpioPinConfig::PinEnum::Pin0:
		{
			Interrupt::DisableIRQ(IRQn_Type::EXTI0_IRQn);
			_on_exti0_interrupt = nullptr;
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin1:
		{
			Interrupt::DisableIRQ(IRQn_Type::EXTI1_IRQn);
			_on_exti1_interrupt = nullptr;
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin2:
		{
			Interrupt::DisableIRQ(IRQn_Type::EXTI2_IRQn);
			_on_exti2_interrupt = nullptr;
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin3:
		{
			Interrupt::DisableIRQ(IRQn_Type::EXTI3_IRQn);
			_on_exti3_interrupt = nullptr;
			break;
		}
	case hal::GpioPinConfig::PinEnum::Pin4:
		{
			Interrupt::DisableIRQ(IRQn_Type::EXTI4_IRQn);
			_on_exti4_interrupt = nullptr;
			break;
		}
	default:
		{
			throw std::invalid_argument { "pin 超出范围。" };
		}
	}
}
