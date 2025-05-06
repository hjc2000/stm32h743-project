#include "hal.h"

extern "C"
{
	uint32_t freertos_get_systic_clock_freq(uint8_t sync_to_cpu)
	{
		/* 无论 sync_to_cpu 是什么，都应该返回系统时钟频率，因为 stm32h743 不支持让
		 * SysTick 与 CPU 不同频率。
		 */
		uint32_t freq = HAL_RCC_GetSysClockFreq();
		return freq;
	}
}
