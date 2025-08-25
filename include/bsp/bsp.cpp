#include "bsp.h"
#include "base/Console.h"
#include "base/task/delay.h"
#include "hal-wrapper/peripheral/timer/PwmModeTimer3.h"
#include <cstdint>

void TestUniversalTimer1()
{
	hal::UniversalTimerBaseConfig config;
	config.SetPrescaler(200 - 1);
	config.SetPeriod(500 - 1);
	config.SetCounterMode(hal::UniversalTimerBaseConfig::CounterModeOption::UP);
	config.SetClockDivision(hal::UniversalTimerBaseConfig::ClockDivisionOption::DIV1);
	hal::PwmModeTimer3::Instance().PwmInitialize(config);
	hal::UniversalTimerCompareOutputConfig compare_output_config;
	compare_output_config.SetMode(hal::UniversalTimerCompareOutputConfig::ModeOption::Pwm1);
	compare_output_config.SetPulse(500 / 2);
	compare_output_config.SetPolarity(hal::UniversalTimerCompareOutputConfig::PolarityOption::Low);

	hal::PwmModeTimer3::Instance().ConfigPwmChannel(compare_output_config,
													hal::TimerChannelEnum::Channel4);

	hal::PwmModeTimer3::Instance().StartPwm(hal::TimerChannelEnum::Channel4);

	uint32_t value = 500 / 2;
	while (true)
	{
		base::task::Delay(std::chrono::milliseconds{1000});
		value += config.Period() + config.Period() / 4;
		value %= config.Period();
		compare_output_config.SetPulse(value);

		hal::PwmModeTimer3::Instance().ConfigPwmChannel(compare_output_config,
														hal::TimerChannelEnum::Channel4);
	}
}
