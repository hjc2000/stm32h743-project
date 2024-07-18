#include "IGpioPort.h"

using namespace hal;

void IGpioPort::InitPin(GpioPinConfig &config)
{
	HAL_GPIO_Init(HardwareInstance(), &config.Handle());
}

void IGpioPort::DigitalWritePin(GpioPinConfig::PinEnum pin, bool value)
{
	GPIO_PinState state = value ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET;
	HAL_GPIO_WritePin(HardwareInstance(), (uint16_t)pin, state);
}

bool IGpioPort::DigitalReadPin(GpioPinConfig::PinEnum pin)
{
	GPIO_PinState pin_state = HAL_GPIO_ReadPin(HardwareInstance(), (uint16_t)pin);
	if (pin_state == GPIO_PinState::GPIO_PIN_SET)
	{
		return true;
	}

	return false;
}

void IGpioPort::DigitalTogglePin(GpioPinConfig::PinEnum pin)
{
	HAL_GPIO_TogglePin(HardwareInstance(), (uint16_t)pin);
}
