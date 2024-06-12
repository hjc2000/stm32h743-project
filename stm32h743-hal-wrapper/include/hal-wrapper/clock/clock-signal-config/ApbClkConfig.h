#pragma once
#include<hal.h>

namespace hal
{
	class Apb1ClkConfig
	{
	public:
		enum class InputDivider
		{
			DIV1 = RCC_APB1_DIV1,
			DIV2 = RCC_APB1_DIV2,
			DIV4 = RCC_APB1_DIV4,
			DIV8 = RCC_APB1_DIV8,
			DIV16 = RCC_APB1_DIV16,
		};

		InputDivider _input_divider { };

		void Serialize(RCC_ClkInitTypeDef &o) const;
		void Deserialize(RCC_ClkInitTypeDef const &o);
	};

	class Apb2ClkConfig
	{
	public:
		enum class InputDivider
		{
			DIV1 = RCC_APB2_DIV1,
			DIV2 = RCC_APB2_DIV2,
			DIV4 = RCC_APB2_DIV4,
			DIV8 = RCC_APB2_DIV8,
			DIV16 = RCC_APB2_DIV16,
		};

		/// <summary>
		///		为 APB2 或者叫 PCLK2 选择分频系数。
		/// </summary>
		InputDivider _input_divider { };

		void Serialize(RCC_ClkInitTypeDef &o) const;
		void Deserialize(RCC_ClkInitTypeDef const &o);
	};

	class Apb3ClkConfig
	{
	public:
		enum class InputDivider
		{
			DIV1 = RCC_APB3_DIV1,
			DIV2 = RCC_APB3_DIV2,
			DIV4 = RCC_APB3_DIV4,
			DIV8 = RCC_APB3_DIV8,
			DIV16 = RCC_APB3_DIV16,
		};

		InputDivider _input_divider { };

		void Serialize(RCC_ClkInitTypeDef &o) const;
		void Deserialize(RCC_ClkInitTypeDef const &o);
	};

	class Apb4ClkConfig
	{
	public:
		enum class InputDivider
		{
			DIV1 = RCC_APB4_DIV1,
			DIV2 = RCC_APB4_DIV2,
			DIV4 = RCC_APB4_DIV4,
			DIV8 = RCC_APB4_DIV8,
			DIV16 = RCC_APB4_DIV16,
		};

		InputDivider _input_divider { };

		void Serialize(RCC_ClkInitTypeDef &o) const;
		void Deserialize(RCC_ClkInitTypeDef const &o);
	};
}