#include"GpioPort.h"

using namespace bsp;

#pragma region GpioPortA
GPIO_TypeDef *GpioPortA::HardwareInstance()
{
	return GPIOA;
}

bool GpioPortA::IsClockEnabled()
{
	return __HAL_RCC_GPIOA_IS_CLK_ENABLED();
}

void GpioPortA::EnableClock()
{
	if (IsClockEnabled())
	{
		return;
	}

	__HAL_RCC_GPIOA_CLK_ENABLE();
}

void GpioPortA::DisableClock()
{
	if (IsClockDisabled())
	{
		return;
	}

	__HAL_RCC_GPIOA_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortB
GPIO_TypeDef *GpioPortB::HardwareInstance()
{
	return GPIOB;
}

bool GpioPortB::IsClockEnabled()
{
	return __HAL_RCC_GPIOB_IS_CLK_ENABLED();
}

void GpioPortB::EnableClock()
{
	if (IsClockEnabled())
	{
		return;
	}

	__HAL_RCC_GPIOB_CLK_ENABLE();
}

void GpioPortB::DisableClock()
{
	if (IsClockDisabled())
	{
		return;
	}

	__HAL_RCC_GPIOB_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortC
GPIO_TypeDef *GpioPortC::HardwareInstance()
{
	return GPIOC;
}

bool GpioPortC::IsClockEnabled()
{
	return __HAL_RCC_GPIOC_IS_CLK_ENABLED();
}

void GpioPortC::EnableClock()
{
	if (IsClockEnabled())
	{
		return;
	}

	__HAL_RCC_GPIOC_CLK_ENABLE();
}

void GpioPortC::DisableClock()
{
	if (IsClockDisabled())
	{
		return;
	}

	__HAL_RCC_GPIOC_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortD
GPIO_TypeDef *GpioPortD::HardwareInstance()
{
	return GPIOD;
}

bool GpioPortD::IsClockEnabled()
{
	return __HAL_RCC_GPIOD_IS_CLK_ENABLED();
}

void GpioPortD::EnableClock()
{
	if (IsClockEnabled())
	{
		return;
	}

	__HAL_RCC_GPIOD_CLK_ENABLE();
}

void GpioPortD::DisableClock()
{
	if (IsClockDisabled())
	{
		return;
	}

	__HAL_RCC_GPIOD_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortE
GPIO_TypeDef *GpioPortE::HardwareInstance()
{
	return GPIOE;
}

bool GpioPortE::IsClockEnabled()
{
	return __HAL_RCC_GPIOE_IS_CLK_ENABLED();
}

void GpioPortE::EnableClock()
{
	if (IsClockEnabled())
	{
		return;
	}

	__HAL_RCC_GPIOE_CLK_ENABLE();
}

void GpioPortE::DisableClock()
{
	if (IsClockDisabled())
	{
		return;
	}

	__HAL_RCC_GPIOE_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortF
GPIO_TypeDef *GpioPortF::HardwareInstance()
{
	return GPIOF;
}

bool GpioPortF::IsClockEnabled()
{
	return __HAL_RCC_GPIOF_IS_CLK_ENABLED();
}

void GpioPortF::EnableClock()
{
	if (IsClockEnabled())
	{
		return;
	}

	__HAL_RCC_GPIOF_CLK_ENABLE();
}

void GpioPortF::DisableClock()
{
	if (IsClockDisabled())
	{
		return;
	}

	__HAL_RCC_GPIOF_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortG
GPIO_TypeDef *GpioPortG::HardwareInstance()
{
	return GPIOG;
}

bool GpioPortG::IsClockEnabled()
{
	return __HAL_RCC_GPIOG_IS_CLK_ENABLED();
}

void GpioPortG::EnableClock()
{
	if (IsClockEnabled())
	{
		return;
	}

	__HAL_RCC_GPIOG_CLK_ENABLE();
}

void GpioPortG::DisableClock()
{
	if (IsClockDisabled())
	{
		return;
	}

	__HAL_RCC_GPIOG_CLK_DISABLE();
}
#pragma endregion
