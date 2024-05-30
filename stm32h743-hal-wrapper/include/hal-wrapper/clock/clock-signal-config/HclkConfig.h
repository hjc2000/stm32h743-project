#pragma once
#include<hal-wrapper/clock/clock-signal-config/ApbClkConfig.h>

namespace hal
{
	class HclkConfig
	{
	public:
		enum class InputDivider
		{
			DIV1 = RCC_HCLK_DIV1,
			DIV2 = RCC_HCLK_DIV2,
			DIV4 = RCC_HCLK_DIV4,
			DIV8 = RCC_HCLK_DIV8,
			DIV16 = RCC_HCLK_DIV16,
			DIV64 = RCC_HCLK_DIV64,
			DIV128 = RCC_HCLK_DIV128,
			DIV256 = RCC_HCLK_DIV256,
			DIV512 = RCC_HCLK_DIV512,
		};

		/// <summary>
		///		HCLK 输入端的分频器。
		/// </summary>
		InputDivider _input_divider;

		#pragma region 子时钟
		Apb1ClkConfig _apb1clk_config;
		Apb2ClkConfig _apb2clk_config;
		Apb3ClkConfig _apb3clk_config;
		Apb4ClkConfig _apb4clk_config;
		#pragma endregion

		void Serialize(RCC_ClkInitTypeDef &o) const;
		void Deserialize(RCC_ClkInitTypeDef const &o);
	};
}