#pragma once
#include<hal.h>

namespace bsp
{
	class ClockSignalConfig
	{
	public:
		ClockSignalConfig() = default;
		ClockSignalConfig(RCC_ClkInitTypeDef const &o);
		ClockSignalConfig &operator=(RCC_ClkInitTypeDef const &o);
		operator RCC_ClkInitTypeDef() const;

		/// <summary>
		///		选择要设置的时钟信号类型。
		///		本枚举类型的枚举值可以按位或，从而选中多个时钟。
		/// </summary>
		enum class ClockType :uint32_t
		{
			SYSCLK = RCC_CLOCKTYPE_SYSCLK,
			HCLK = RCC_CLOCKTYPE_HCLK,
			PCLK1 = RCC_CLOCKTYPE_PCLK1,
			PCLK2 = RCC_CLOCKTYPE_PCLK2,
			D1PCLK1 = RCC_CLOCKTYPE_D1PCLK1,
			D3PCLK1 = RCC_CLOCKTYPE_D3PCLK1,
		};

		ClockType _clock_type;

		/// <summary>
		///		选择系统时钟的时钟源
		/// </summary>
		enum class SysclkSource :uint32_t
		{
			CSI = RCC_SYSCLKSOURCE_CSI,
			HSI = RCC_SYSCLKSOURCE_HSI,
			HSE = RCC_SYSCLKSOURCE_HSE,
			PLLCLK = RCC_SYSCLKSOURCE_PLLCLK,
		};

		SysclkSource _sysclk_source;

		/// <summary>
		///		AHB 分频器的分频系数。
		///		SYSCLK 经过 AHB 分频器后变成 HCLK。
		/// </summary>
		enum class AHBDivider :uint32_t
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
		///		APB1 和 APB2 分频器的分频系数。
		///		HCLK 经过 APB1 分频器后变成 PCLK1，此即低速外设时钟。
		///		HCLK 经过 APB2 分频器后变成 PCLK2，此即高速外设时钟。
		/// </summary>
		enum class APBDivider :uint32_t
		{
			DIV1 = RCC_HCLK_DIV1,
			DIV2 = RCC_HCLK_DIV2,
			DIV4 = RCC_HCLK_DIV4,
			DIV8 = RCC_HCLK_DIV8,
			DIV16 = RCC_HCLK_DIV16,
		};

		/// <summary>
		///		AHB 分频器。分频后输出 HCLK。
		/// </summary>
		AHBDivider _ahb_clk_divider;

		/// <summary>
		///		APB1 分频器的分频系数。分频后输出 PCLK1，此即低速外设时钟。
		/// </summary>
		APBDivider _apb1_divider;

		/// <summary>
		///		APB2 分频器的分频系数。分频后输出 PCLK2，此即高速外设时钟。
		/// </summary>
		APBDivider _apb2_divider;

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
			/// <summary>
			///		延迟 0 个 CPU 时钟周期。
			/// </summary>
			Latency0 = FLASH_LATENCY_0,

			/// <summary>
			///		延迟 1 个 CPU 时钟周期。
			/// </summary>
			Latency1 = FLASH_LATENCY_1,

			/// <summary>
			///		延迟 2 个 CPU 时钟周期。
			/// </summary>
			Latency2 = FLASH_LATENCY_2,
		};

		FlashLatency _flash_latency;
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
