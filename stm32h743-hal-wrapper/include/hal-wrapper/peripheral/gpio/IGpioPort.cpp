#include"IGpioPort.h"

using namespace hal;

void IGpioPort::InitPin(GpioPin pin, GpioPinConfig const &config)
{
	GPIO_InitTypeDef gpio_init_options { };
	gpio_init_options.Pin = static_cast<uint32_t>(pin);
	gpio_init_options.Mode = static_cast<uint32_t>(config._mode);
	gpio_init_options.Pull = static_cast<uint32_t>(config._pull_mode);
	gpio_init_options.Speed = static_cast<uint32_t>(config._speed);
	gpio_init_options.Alternate = config._alternate;
	HAL_GPIO_Init(HardwareInstance(), &gpio_init_options);
}

void IGpioPort::DigitalWritePin(GpioPin pin, bool value)
{
	GPIO_PinState state = value ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET;
	HAL_GPIO_WritePin(HardwareInstance(), (uint16_t)pin, state);
}

bool IGpioPort::DigitalReadPin(GpioPin pin)
{
	GPIO_PinState pin_state = HAL_GPIO_ReadPin(HardwareInstance(), (uint16_t)pin);
	if (pin_state == GPIO_PinState::GPIO_PIN_SET)
	{
		return true;
	}

	return false;
}

void IGpioPort::DigitalTogglePin(GpioPin pin)
{
	HAL_GPIO_TogglePin(HardwareInstance(), (uint16_t)pin);
}
