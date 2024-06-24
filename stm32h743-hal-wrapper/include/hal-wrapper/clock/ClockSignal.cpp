#include "ClockSignal.h"
#include <stdexcept>

using namespace hal;

ClockSignalConfig hal::ClockSignal::GetConfig()
{
	RCC_ClkInitTypeDef def;
	uint32_t flash_latency;
	HAL_RCC_GetClockConfig(&def, &flash_latency);
	ClockSignalConfig ret{def};
	ret._flash_latency = static_cast<ClockSignalConfig::FlashLatency>(flash_latency);
	return ret;
}

void hal::ClockSignal::SetConfig(ClockSignalConfig const &config)
{
	RCC_ClkInitTypeDef rcc_clk_init = config;
	HAL_StatusTypeDef ret = HAL_RCC_ClockConfig(
		&rcc_clk_init,
		static_cast<uint32_t>(config._flash_latency));

	if (ret != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"时钟信号配置失败"};
	}
}
