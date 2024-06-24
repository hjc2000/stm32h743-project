#pragma once
#include <hal.h>

namespace hal
{
	/// <summary>
	///		PLL 初始化选项
	/// </summary>
	class PllConfig
	{
	public:
		PllConfig() = default;
		PllConfig(RCC_PLLInitTypeDef const &o);
		PllConfig &operator=(RCC_PLLInitTypeDef const &o);
		operator RCC_PLLInitTypeDef() const;

		enum class PllState
		{
			None = RCC_PLL_NONE,
			Off = RCC_PLL_OFF,
			On = RCC_PLL_ON,
		};

		PllState _state = PllState::None;

		enum class PllSource
		{
			HSI = RCC_PLLSOURCE_HSI,
			CSI = RCC_PLLSOURCE_CSI,
			HSE = RCC_PLLSOURCE_HSE,
			NONE = RCC_PLLSOURCE_NONE,
		};

		PllSource _source = PllSource::NONE;

		/// <summary>
		///		取值范围：[1, 63]
		/// </summary>
		uint32_t _m = 5;

		/// <summary>
		///		取值范围：[4, 512]
		/// </summary>
		uint32_t _n = 192;

		/// <summary>
		///		取值范围：[2, 128]
		/// </summary>
		uint32_t _p = 2;

		/// <summary>
		///		取值范围：[1, 128]
		/// </summary>
		uint32_t _q = 2;

		/// <summary>
		///		取值范围：[1, 128]
		/// </summary>
		uint32_t _r = 2;

		enum class Pll1VciRange
		{
			/// <summary>
			///		Clock range frequency between 1 and 2 MHz
			/// </summary>
			Range0 = RCC_PLL1VCIRANGE_0,

			/// <summary>
			///		Clock range frequency between 2 and 4 MHz
			/// </summary>
			Range1 = RCC_PLL1VCIRANGE_1,

			/// <summary>
			///		Clock range frequency between 4 and 8 MHz
			/// </summary>
			Range2 = RCC_PLL1VCIRANGE_2,

			/// <summary>
			///		Clock range frequency between 8 and 16 MHz
			/// </summary>
			Range3 = RCC_PLL1VCIRANGE_3,
		};

		Pll1VciRange _vci_range = Pll1VciRange::Range0;

		enum class Pll1VcoRange
		{
			Wide = RCC_PLL1VCOWIDE,
			Medium = RCC_PLL1VCOMEDIUM,
		};

		Pll1VcoRange _vco_range = Pll1VcoRange::Wide;

		/// <summary>
		///		范围：[0, 8191]
		/// </summary>
		uint32_t _fracn = 0;
	};
}
