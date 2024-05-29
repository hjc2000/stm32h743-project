#pragma once
#include<hal.h>

namespace bsp
{
	class ClockSignalConfig
	{
	public:
		ClockSignalConfig();
		ClockSignalConfig(RCC_ClkInitTypeDef const &o);
		ClockSignalConfig &operator=(RCC_ClkInitTypeDef const &o);
		operator RCC_ClkInitTypeDef() const;

		enum class ClockType
		{
			SYSCLK = RCC_CLOCKTYPE_SYSCLK,
			HCLK = RCC_CLOCKTYPE_HCLK,
			D1PCLK1 = RCC_CLOCKTYPE_D1PCLK1,
			PCLK1 = RCC_CLOCKTYPE_PCLK1,
			PCLK2 = RCC_CLOCKTYPE_PCLK2,
			D3PCLK1 = RCC_CLOCKTYPE_D3PCLK1,
		};

		/// <summary>
		///		选择色要配置哪个时钟。可以将多个枚举值按位或。
		/// </summary>
		ClockType _clock_type;

		/// <summary>
		///		SysTick 可选的时钟源。
		/// </summary>
		enum class SysTickClockSource
		{
			CSI = RCC_SYSCLKSOURCE_CSI,
			HSI = RCC_SYSCLKSOURCE_HSI,
			HSE = RCC_SYSCLKSOURCE_HSE,
			PLLCLK = RCC_SYSCLKSOURCE_PLLCLK,
		};

		/// <summary>
		///		为 SysTick 选择时钟源。
		/// </summary>
		SysTickClockSource _systick_clock_source;

		/// <summary>
		///		SysTick 可选的分频系数
		/// </summary>
		enum class SysTickDivider
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

		/// <summary>
		///		为 SysTick 选择分频系数。
		/// </summary>
		SysTickDivider systick_divider;

		/// <summary>
		///		以 HCLK 为时钟源的时钟信号可以选择让 HCLK 多少分频后再进来。
		/// </summary>
		enum class HclkDivider
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

		HclkDivider _ahb_clk_divider;

		/// <summary>
		///		APB3 CLK 可选的分频系数。
		/// </summary>
		enum class Apb3Divider
		{
			DIV1 = RCC_APB3_DIV1,
			DIV2 = RCC_APB3_DIV2,
			DIV4 = RCC_APB3_DIV4,
			DIV8 = RCC_APB3_DIV8,
			DIV16 = RCC_APB3_DIV16,
		};

		/// <summary>
		///		为 APB3 CLK 或者叫 D1PCLK1 选择分频系数。
		/// </summary>
		Apb3Divider _apb3_clk_divider;

		enum class Apb1Divider
		{
			DIV1 = RCC_APB1_DIV1,
			DIV2 = RCC_APB1_DIV2,
			DIV4 = RCC_APB1_DIV4,
			DIV8 = RCC_APB1_DIV8,
			DIV16 = RCC_APB1_DIV16,
		};

		/// <summary>
		///		为 APB1 CLK 或者叫 PCLK1 选择分频系数。
		/// </summary>
		Apb1Divider _apb1_clk_divider;

		enum class Apb2Divider
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
		Apb2Divider _apb2_clk_divider;

		enum class Apb4Divider
		{
			DIV1 = RCC_APB4_DIV1,
			DIV2 = RCC_APB4_DIV2,
			DIV4 = RCC_APB4_DIV4,
			DIV8 = RCC_APB4_DIV8,
			DIV16 = RCC_APB4_DIV16,
		};

		Apb4Divider _apb4_clk_divider;

		/// <summary>
		///		读写 flash 时要延迟多少个 CPU 时钟周期。
		///		CPU 时钟是由 HCLK 再经过一个分频器提供的。在 stm32f103 中，
		///		这个分频器可以选择 1 分频或 8 分频。
		/// 
		///		因为 flash 是低速储存器，CPU 时钟频率很高时会出现速度不匹配，
		///		这时候需要 CPU 通过延时来等待 flash。
		/// </summary>
		enum class FlashLatency
		{
			Latency0 = FLASH_LATENCY_0,
			Latency1 = FLASH_LATENCY_1,
			Latency2 = FLASH_LATENCY_2,
			Latency3 = FLASH_LATENCY_3,
			Latency4 = FLASH_LATENCY_4,
			Latency5 = FLASH_LATENCY_5,
			Latency6 = FLASH_LATENCY_6,
			Latency7 = FLASH_LATENCY_7,
		};

		FlashLatency _flash_latency = FlashLatency::Latency2;
	};
}

/// <summary>
///		让 ClockType 枚举类型支持按位或。
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
/// <returns></returns>
bsp::ClockSignalConfig::ClockType operator|(
	bsp::ClockSignalConfig::ClockType left,
	bsp::ClockSignalConfig::ClockType right
	);
