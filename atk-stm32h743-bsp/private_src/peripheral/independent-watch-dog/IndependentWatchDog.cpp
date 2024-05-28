#include"IndependentWatchDog.h"

using namespace bsp;

void bsp::IndependentWatchDog::Initialize(IndependentWatchDogInitOptions const &options)
{
	Handle()->Instance = HardwareInstance();
	Handle()->Init = options;
	HAL_IWDG_Init(Handle());
}

uint32_t bsp::IndependentWatchDog::PrescalerValue()
{
	switch (Prescaler())
	{
	case IndependentWatchDogPrescaler::Div4:
		{
			return 4;
		}
	case IndependentWatchDogPrescaler::Div8:
		{
			return 8;
		}
	case IndependentWatchDogPrescaler::Div16:
		{
			return 16;
		}
	case IndependentWatchDogPrescaler::Div32:
		{
			return 32;
		}
	case IndependentWatchDogPrescaler::Div64:
		{
			return 64;
		}
	case IndependentWatchDogPrescaler::Div128:
		{
			return 128;
		}
	case IndependentWatchDogPrescaler::Div256:
		{
			return 256;
		}
	default:
		{
			// 这里不返回 0，防止调用者拿返回值去做除数
			return 1;
		}
	}
}

std::chrono::milliseconds bsp::IndependentWatchDog::WatchDogTimeoutDuration()
{
	return std::chrono::milliseconds{
		(uint64_t)1000 * Handle()->Init.Reload * InnerClockSourceFreq_Hz() / PrescalerValue()
	};
}

void bsp::IndependentWatchDog::SetWatchDogTimeoutDuration(std::chrono::milliseconds value)
{
	/*
	* 计数器的计数周期为：
	*	T = 1 / CounterFreq_Hz()
	* 单位是秒。
	* value 化成秒是：
	*	seconds = value / 1000
	* 想要持续 seconds 秒，计数值是：
	*	count = seconds / T
	*	count = value / 1000 / T
	*	count = value / 1000 * CounterFreq_Hz()
	* 先乘再除可以防止截断误差
	*	count = value * CounterFreq_Hz() / 1000
	*	count = value * InnerClockSourceFreq_Hz() / PrescalerValue() / 1000
	*	count * PrescalerValue() = value * InnerClockSourceFreq_Hz() / 1000
	*
	* count * PrescalerValue() 是具有物理意义的。分频器也可以看作是计数器，分频器计数满了，
	* 进位给计数器，就实现分频了。看门狗的分频系数都是 2 的整数次幂，从 4 到 256，我们只需要
	* 找出不让计数器溢出的最小分频系数。
	*/

	// 所需的分频器和计数器总共的计数值
	uint64_t needed_counter_and_prescaler_value = value.count() * InnerClockSourceFreq_Hz() / 1000;
	uint64_t needed_counter_value = 0;
	IndependentWatchDogPrescaler needed_prescaler = IndependentWatchDogPrescaler::Div256;
	for (uint16_t i = 2; i <= 8; i++)
	{
		// 从 2^2 = 4 开始，到 2^8 = 256，通过移位实现幂。i 代表的是 2 的幂
		uint16_t prescaler_value = (uint16_t)1 << i;
		needed_counter_value = needed_counter_and_prescaler_value / prescaler_value;

		if (needed_counter_value > 0X0FFF && i == 8)
		{
			// 最大分频和最大计数都无法表示这个时间，就按照能达到的最大值来。
			needed_counter_value = 0X0FFF;
			needed_prescaler = IndependentWatchDogPrescaler::Div256;
			break;
		}

		if (needed_counter_value <= 0x0FFF)
		{
			// i 代表的是 2 的幂，将 i 映射到分频系数枚举值
			needed_prescaler = PowerToIndependentWatchDogPrescaler(i);
			break;
		}
	}

	IndependentWatchDogInitOptions options;
	options._prescaler = needed_prescaler;
	options._reload = (uint32_t)needed_counter_value;
	Initialize(options);
}

void bsp::IndependentWatchDog::Feed()
{
	HAL_IWDG_Refresh(Handle());
}
