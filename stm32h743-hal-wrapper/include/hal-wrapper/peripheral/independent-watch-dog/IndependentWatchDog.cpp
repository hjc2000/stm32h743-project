#include"IndependentWatchDog.h"
#include<base/math/Fraction.h>

using namespace hal;
using namespace base;

void IndependentWatchDog::Initialize()
{
	_handle.Instance = HardwareInstance();
	_handle.Init = _config.Handle();
	HAL_IWDG_Init(&_handle);
}

std::chrono::milliseconds IndependentWatchDog::WatchDogTimeoutDuration() const
{
	Fraction count_freq = Fraction { InnerClockSourceFreq_Hz() } / _config.GetPrescalerByUint32();
	Fraction count_interval = 1 / count_freq;
	Fraction timeout_in_seconds = _config.ReloadValue() * count_interval;
	Fraction timeout_in_milliseconds = timeout_in_seconds * 1000;
	return std::chrono::milliseconds { timeout_in_milliseconds.Div() };
}

void IndependentWatchDog::SetWatchDogTimeoutDuration(std::chrono::milliseconds value)
{
	Fraction inner_clock_source_freq = InnerClockSourceFreq_Hz();
	Fraction inner_clock_source_interval = 1 / inner_clock_source_freq;
	Fraction timeout_in_milliseconds = value.count();
	Fraction timeout = timeout_in_milliseconds / 1000;

	// 所需的 (分频器计数值 + 计数器计数值)
	Fraction total_count = timeout / inner_clock_source_interval;
	int64_t int_total_count = total_count.Div();

	// 让计数器重装载值尽量大，溢出了就增大分频系数
	for (uint16_t i = 2; i <= 8; i++)
	{
		// 从 2^2 = 4 开始，到 2^8 = 256，通过移位实现幂。i 代表的是 2 的幂
		uint16_t prescaler_value = static_cast<uint16_t>(1 << i);
		_config.SetReloadValue(int_total_count / prescaler_value);
		if (_config.ReloadValue() > 0X0FFF && i == 8)
		{
			// 最大分频和最大计数都无法表示这个时间，就按照能达到的最大值来。
			_config.SetReloadValue(0X0FFF);
			_config.SetPrescaler(IndependentWatchDogConfig::PrescalerOption::Div256);
			break;
		}

		if (_config.ReloadValue() <= 0x0FFF)
		{
			// i 代表的是 2 的幂，将 i 映射到分频系数枚举值
			_config.SetPrescalerByPow(i);
			break;
		}
	}

	Initialize();
}

void IndependentWatchDog::Feed()
{
	HAL_IWDG_Refresh(&_handle);
}
