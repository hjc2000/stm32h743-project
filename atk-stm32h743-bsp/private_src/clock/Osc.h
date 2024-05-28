#pragma once
#include<hal.h>
#include<OscConfig.h>
#include<PllInitOptions.h>

namespace bsp
{
	/// <summary>
	///		时钟系统只有一个，所以实现为静态类
	/// </summary>
	class Osc
	{
	private:
		Osc() = delete;

	public:
		static OscConfig Config()
		{
			RCC_OscInitTypeDef def;
			HAL_RCC_GetOscConfig(&def);
			return OscConfig{ def };
		}

		/// <summary>
		///		配置时钟源
		/// </summary>
		/// <param name="options"></param>
		/// <returns></returns>
		static void SetConfig(OscConfig const &options)
		{
			RCC_OscInitTypeDef rcc_osc_init = options;
			HAL_StatusTypeDef result = HAL_RCC_OscConfig(&rcc_osc_init);
			if (result != HAL_StatusTypeDef::HAL_OK)
			{
				throw std::runtime_error{ "时钟源配置失败" };
			}
		}
	};
}
