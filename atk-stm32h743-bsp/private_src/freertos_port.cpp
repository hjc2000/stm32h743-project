#include"FreeRTOSConfig.h"
#include<FreeRTOS.h>
#include<Systic.h>
#include<hal.h>
#include<task.h>

extern "C"
{
	uint32_t freertos_get_cpu_freq()
	{
		return HAL_RCC_GetHCLKFreq();
	}

	uint32_t freertos_get_systic_clock_freq()
	{
		return bsp::Systic::Instance().Frequency();
	}

	/// <summary>
	///		位于 libfreertos.a 中的一个函数，并没有暴露到头文件中。
	/// </summary>
	void xPortSysTickHandler();

	void SysTick_Handler()
	{
		HAL_IncTick();
		if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		{
			xPortSysTickHandler();
		}
	}
}
