#include "GpioPort.h"

using namespace hal;

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
	__HAL_RCC_GPIOA_CLK_ENABLE();
}

void GpioPortA::DisableClock()
{
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
	__HAL_RCC_GPIOB_CLK_ENABLE();
}

void GpioPortB::DisableClock()
{
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
	__HAL_RCC_GPIOC_CLK_ENABLE();
}

void GpioPortC::DisableClock()
{
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
	__HAL_RCC_GPIOD_CLK_ENABLE();
}

void GpioPortD::DisableClock()
{
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
	__HAL_RCC_GPIOF_CLK_ENABLE();
}

void GpioPortF::DisableClock()
{
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
	__HAL_RCC_GPIOG_CLK_ENABLE();
}

void GpioPortG::DisableClock()
{
	__HAL_RCC_GPIOG_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortH
GPIO_TypeDef *GpioPortH::HardwareInstance()
{
	return GPIOH;
}

bool GpioPortH::IsClockEnabled()
{
	return __HAL_RCC_GPIOH_IS_CLK_ENABLED();
}

void GpioPortH::EnableClock()
{
	__HAL_RCC_GPIOH_CLK_ENABLE();
}

void GpioPortH::DisableClock()
{
	__HAL_RCC_GPIOH_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortI
GPIO_TypeDef *GpioPortI::HardwareInstance()
{
	return GPIOI;
}

bool GpioPortI::IsClockEnabled()
{
	return __HAL_RCC_GPIOI_IS_CLK_ENABLED();
}

void GpioPortI::EnableClock()
{
	__HAL_RCC_GPIOI_CLK_ENABLE();
}

void GpioPortI::DisableClock()
{
	__HAL_RCC_GPIOI_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortJ
GPIO_TypeDef *GpioPortJ::HardwareInstance()
{
	return GPIOJ;
}

bool GpioPortJ::IsClockEnabled()
{
	return __HAL_RCC_GPIOJ_IS_CLK_ENABLED();
}

void GpioPortJ::EnableClock()
{
	__HAL_RCC_GPIOJ_CLK_ENABLE();
}

void GpioPortJ::DisableClock()
{
	__HAL_RCC_GPIOJ_CLK_DISABLE();
}
#pragma endregion

#pragma region GpioPortK
GPIO_TypeDef *GpioPortK::HardwareInstance()
{
	return GPIOK;
}

bool GpioPortK::IsClockEnabled()
{
	return __HAL_RCC_GPIOK_IS_CLK_ENABLED();
}

void GpioPortK::EnableClock()
{
	__HAL_RCC_GPIOK_CLK_ENABLE();
}

void GpioPortK::DisableClock()
{
	__HAL_RCC_GPIOK_CLK_DISABLE();
}
#pragma endregion
