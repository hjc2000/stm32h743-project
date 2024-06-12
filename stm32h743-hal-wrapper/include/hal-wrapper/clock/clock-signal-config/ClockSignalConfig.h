#pragma once
#include<hal-wrapper/clock/clock-signal-config/SystemClockConfig.h>

namespace hal
{
	/// <summary>
	///		根类型。上方都是本类的子成员的类型。
	/// </summary>
	class ClockSignalConfig
	{
	public:
		ClockSignalConfig() = default;
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
		ClockType _clock_type { };

		/// <summary>
		///		选中所有时钟类型。
		/// </summary>
		void SelectAllClockType();

		SystemClockConfig _system_clk_config { };

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

		void Serialize(RCC_ClkInitTypeDef &o) const;
		void Deserialize(RCC_ClkInitTypeDef const &o);
	};
}

/// <summary>
///		让 ClockType 枚举类型支持按位或。
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
/// <returns></returns>
hal::ClockSignalConfig::ClockType operator|(
	hal::ClockSignalConfig::ClockType left,
	hal::ClockSignalConfig::ClockType right
	);
