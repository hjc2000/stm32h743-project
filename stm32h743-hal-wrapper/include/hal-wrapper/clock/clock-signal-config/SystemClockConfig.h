#pragma once
#include <hal-wrapper/clock/clock-signal-config/HclkConfig.h>

namespace hal
{
	/// @brief SYSCLK
	/// @note 直接连着时钟源的一个时钟信号。
	class SystemClockConfig
	{
	public:
		enum class ClockSource
		{
			CSI = RCC_SYSCLKSOURCE_CSI,
			HSI = RCC_SYSCLKSOURCE_HSI,
			HSE = RCC_SYSCLKSOURCE_HSE,
			PLLCLK = RCC_SYSCLKSOURCE_PLLCLK,
		};

		/// @brief 选择 SYSCLK 的输入时钟源
		ClockSource _clock_source{};

		enum class OutputDivider
		{
			DIV1 = RCC_SYSCLK_DIV1,
			DIV2 = RCC_SYSCLK_DIV2,
			DIV4 = RCC_SYSCLK_DIV4,
			DIV8 = RCC_SYSCLK_DIV8,
			DIV16 = RCC_SYSCLK_DIV16,
			DIV64 = RCC_SYSCLK_DIV64,
			DIV128 = RCC_SYSCLK_DIV128,
			DIV256 = RCC_SYSCLK_DIV256,
			DIV512 = RCC_SYSCLK_DIV512,
		};

		/// @brief SYSCLK 输出的分频系数
		OutputDivider _output_divider{};

#pragma region 子时钟信号
		HclkConfig _hclk_config{};
#pragma endregion

		void Serialize(RCC_ClkInitTypeDef &o) const;
		void Deserialize(RCC_ClkInitTypeDef const &o);
	};
}