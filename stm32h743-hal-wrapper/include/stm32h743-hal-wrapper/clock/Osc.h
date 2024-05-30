#pragma once
#include<hal.h>
#include<stdexcept>
#include<stm32h743-hal-wrapper/clock/OscConfig.h>
#include<stm32h743-hal-wrapper/clock/PllConfig.h>

namespace hal
{
	class Osc
	{
	private:
		Osc() = delete;

	public:
		static hal::OscConfig GetConfig()
		{
			RCC_OscInitTypeDef def;
			HAL_RCC_GetOscConfig(&def);
			return hal::OscConfig { def };
		}

		/// <summary>
		///		配置时钟源
		/// </summary>
		/// <param name="options"></param>
		/// <returns></returns>
		static void SetConfig(hal::OscConfig const &options)
		{
			RCC_OscInitTypeDef rcc_osc_init = options;
			HAL_StatusTypeDef result = HAL_RCC_OscConfig(&rcc_osc_init);
			if (result != HAL_StatusTypeDef::HAL_OK)
			{
				throw std::runtime_error { "时钟源配置失败" };
			}
		}
	};
}
