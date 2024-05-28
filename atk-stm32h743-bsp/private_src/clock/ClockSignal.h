#pragma once
#include<ClockSignalConfig.h>

namespace bsp
{
	class ClockSignal
	{
	private:
		ClockSignal() = delete;
		ClockSignal(ClockSignal const &o) = delete;
		ClockSignal(ClockSignal const &&o) = delete;
		ClockSignal &operator=(ClockSignal const &o) = delete;

	public:
		/// <summary>
		///		获取当前时钟信号的配置。
		/// </summary>
		/// <returns></returns>
		static ClockSignalConfig Config();

		/// <summary>
		///		应用时钟信号配置。
		/// </summary>
		/// <param name="config"></param>
		/// <param name="flash_latency"></param>
		/// <returns></returns>
		static void SetConfig(ClockSignalConfig const &config);

		static uint32_t HclkFreq()
		{
			return HAL_RCC_GetHCLKFreq();
		}

		static uint32_t Pclk1Freq()
		{
			return HAL_RCC_GetPCLK1Freq();
		}

		static uint32_t Pclk2Freq()
		{
			return HAL_RCC_GetPCLK2Freq();
		}
	};
}
