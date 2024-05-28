#pragma once
#include<hal.h>

namespace bsp
{
	/// <summary>
	///		定义 GPIO 引脚。
	/// </summary>
	enum class GpioPin : uint16_t
	{
		Pin0 = GPIO_PIN_0,
		Pin1 = GPIO_PIN_1,
		Pin2 = GPIO_PIN_2,
		Pin3 = GPIO_PIN_3,
		Pin4 = GPIO_PIN_4,
		Pin5 = GPIO_PIN_5,
		Pin6 = GPIO_PIN_6,
		Pin7 = GPIO_PIN_7,
		Pin8 = GPIO_PIN_8,
		Pin9 = GPIO_PIN_9,
		Pin10 = GPIO_PIN_10,
		Pin11 = GPIO_PIN_11,
		Pin12 = GPIO_PIN_12,
		Pin13 = GPIO_PIN_13,
		Pin14 = GPIO_PIN_14,
		Pin15 = GPIO_PIN_15,
	};

	/// <summary>
	///		定义 GPIO 引脚模式。
	/// </summary>
	enum class GpioPinMode :uint32_t
	{
		Input = GPIO_MODE_INPUT,

		Output_PushPull = GPIO_MODE_OUTPUT_PP,
		Output_OpenDrain = GPIO_MODE_OUTPUT_OD,

		AlternateFunction_PushPull = GPIO_MODE_AF_PP,
		AlternateFunction_OpenDrain = GPIO_MODE_AF_OD,

		Analog = GPIO_MODE_ANALOG,

		Interrupt_RisingEdgeTrigger = GPIO_MODE_IT_RISING,
		Interrupt_FallingEdgeTrigger = GPIO_MODE_IT_FALLING,
		Interrupt_BothEdgeTrigger = GPIO_MODE_IT_RISING_FALLING,

		Event_RisingEdgeTrigger = GPIO_MODE_EVT_RISING,
		Event_FallingEdgeTrigger = GPIO_MODE_EVT_FALLING,
		Event_BothEdgeTrigger = GPIO_MODE_EVT_RISING_FALLING,
	};

	/// <summary>
	///		定义 GPIO 引脚的上下拉模式。
	/// </summary>
	enum class GpioPinPull :uint32_t
	{
		NoPull = GPIO_NOPULL,
		PullUp = GPIO_PULLUP,
		PullDown = GPIO_PULLDOWN,
	};

	/// <summary>
	///		定义 GPIO 引脚的最大速度。
	/// </summary>
	enum GpioPinSpeed :uint32_t
	{
		Low = GPIO_SPEED_FREQ_LOW,
		Medium = GPIO_SPEED_FREQ_MEDIUM,
		High = GPIO_SPEED_FREQ_HIGH,
		VeryHigh = GPIO_SPEED_FREQ_VERY_HIGH,
	};

	class GpioPinInitOptions
	{
	public:
		GpioPinMode _mode = GpioPinMode::Input;
		GpioPinPull _pull_mode = GpioPinPull::NoPull;
		GpioPinSpeed _speed = GpioPinSpeed::High;
	};
}
