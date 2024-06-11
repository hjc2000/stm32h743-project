#pragma once
#include<hal.h>

namespace hal
{
	enum class TimerChannelEnum
	{
		Channel1 = TIM_CHANNEL_1,
		Channel2 = TIM_CHANNEL_2,
		Channel3 = TIM_CHANNEL_3,
		Channel4 = TIM_CHANNEL_4,
		Channel5 = TIM_CHANNEL_5,
		Channel6 = TIM_CHANNEL_6,
		All = TIM_CHANNEL_ALL,
	};
}