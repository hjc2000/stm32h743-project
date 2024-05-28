#pragma once
#include<hal.h>
#include<PllConfig.h>

namespace bsp
{
	/// <summary>
	///		振荡器配置。（或者说时钟源配置）
	/// </summary>
	class OscConfig
	{
	public:
		OscConfig() = default;
		OscConfig(RCC_OscInitTypeDef const &def);
		OscConfig &operator=(RCC_OscInitTypeDef const &value);
		operator RCC_OscInitTypeDef() const;

		enum class OscillatorType
		{
			NONE = RCC_OSCILLATORTYPE_NONE,
			HSE = RCC_OSCILLATORTYPE_HSE,
			HSI = RCC_OSCILLATORTYPE_HSI,
			LSE = RCC_OSCILLATORTYPE_LSE,
			LSI = RCC_OSCILLATORTYPE_LSI,
		};

		OscillatorType _oscillator_type = OscillatorType::HSI;

		enum class HseState
		{
			Off = RCC_HSE_OFF,
			On = RCC_HSE_ON,
			Bypass = RCC_HSE_BYPASS,
		};

		HseState _hse_state = HseState::Off;

		enum class HsePrediv
		{
			DIV1 = RCC_HSE_PREDIV_DIV1,
			DIV2 = RCC_HSE_PREDIV_DIV2,
		};

		HsePrediv _hse_prediv = HsePrediv::DIV1;

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
		};

		HsiState _hsi_state = HsiState::On;

		enum class LsiState
		{
			Off = RCC_LSI_OFF,
			On = RCC_LSI_ON,
		};

		/// <summary>
		///		必须在区间 [0x00, 0x1f] 上
		/// </summary>
		uint32_t _hsi_calibration_value = RCC_HSICALIBRATION_DEFAULT;

		LsiState _lsi_state = LsiState::Off;

		PllConfig _pll_init_options;
	};
}
