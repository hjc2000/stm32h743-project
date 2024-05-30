#pragma once
#include<hal.h>
#include<hal-wrapper/clock/PllConfig.h>

namespace hal
{
	/// <summary>
	///		振荡器配置。（或者说时钟源配置）
	/// </summary>
	class OscConfig
	{
	public:
		OscConfig() = default;
		OscConfig(RCC_OscInitTypeDef const &o);
		OscConfig &operator=(RCC_OscInitTypeDef const &o);
		operator RCC_OscInitTypeDef() const;

		enum class OscillatorType
		{
			NONE = RCC_OSCILLATORTYPE_NONE,
			HSE = RCC_OSCILLATORTYPE_HSE,
			HSI = RCC_OSCILLATORTYPE_HSI,
			LSE = RCC_OSCILLATORTYPE_LSE,
			LSI = RCC_OSCILLATORTYPE_LSI,
			CSI = RCC_OSCILLATORTYPE_CSI,
			HSI48 = RCC_OSCILLATORTYPE_HSI48,
		};

		OscillatorType _oscillator_type = OscillatorType::NONE;

		enum class HseState
		{
			Off = RCC_HSE_OFF,
			On = RCC_HSE_ON,
			Bypass = RCC_HSE_BYPASS,
		};

		HseState _hse_state = HseState::Off;

		enum class LseState
		{
			Off = RCC_LSE_OFF,
			On = RCC_LSE_ON,
			Bypass = RCC_LSE_BYPASS,
		};

		LseState _lse_state = LseState::Off;

		enum class HsiState
		{
			Off = RCC_HSI_OFF,
			On = RCC_HSI_ON,
			Div1 = RCC_HSI_DIV1,
			Div2 = RCC_HSI_DIV2,
			Div4 = RCC_HSI_DIV4,
			Div8 = RCC_HSI_DIV8,
			CalibrationDefault = RCC_HSICALIBRATION_DEFAULT,
		};

		HsiState _hsi_state = HsiState::Off;

		/// <summary>
		///		取值范围：
		///			STM32H7 rev.Y [0x00, 0x3f]
		///			TM32H7 rev.B and above [0x00, 0x7f]
		///		rev 是 revision 的缩写，即修订版。在 Y 修订版中，取值范围是 [0x00, 0x3f]
		///		在 B 及以上的修订版中，取值范围是 [0x00, 0x7f]
		/// </summary>
		uint32_t _hsi_calibration_value = 0;

		enum class LsiState
		{
			Off = RCC_LSI_OFF,
			On = RCC_LSI_ON,
		};

		LsiState _lsi_state = LsiState::Off;

		enum class Hsi48State
		{
			Off = RCC_HSI48_OFF,
			On = RCC_HSI48_ON,
		};

		Hsi48State _hsi_48_state = Hsi48State::Off;

		enum class CsiState
		{
			Off = RCC_CSI_OFF,
			On = RCC_CSI_ON,
			CalibrationDefault = RCC_CSICALIBRATION_DEFAULT,
		};

		CsiState _csi_state = CsiState::Off;

		/// <summary>
		///		取值范围：
		///			STM32H7 rev.Y [0x00, 0x1f]
		///			TM32H7 rev.B and above [0x00, 0x3f]
		/// </summary>
		uint32_t _csi_calibration_value = 0;

		hal::PllConfig _pll_config;
	};
}
