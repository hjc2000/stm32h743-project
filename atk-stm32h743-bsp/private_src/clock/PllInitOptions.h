#pragma once
#include<hal.h>

namespace bsp
{
	/// <summary>
	///		PLL 状态
	/// </summary>
	enum class PllState :uint32_t
	{
		NotConfigured = RCC_PLL_NONE,
		Off = RCC_PLL_OFF,
		On = RCC_PLL_ON,
	};

	/// <summary>
	///		PLL 时钟源
	/// </summary>
	enum class PllClockSource :uint32_t
	{
		HSI_DIV2 = RCC_PLLSOURCE_HSI_DIV2,
		HSE = RCC_PLLSOURCE_HSE,
	};

	/// <summary>
	///		PLL 倍频系数
	/// </summary>
	enum class PllMul :uint32_t
	{
		#ifdef RCC_PLL_MUL2
		Mul2 = RCC_PLL_MUL2,
		#endif // RCC_PLL_MUL2

		#ifdef RCC_PLL_MUL3
		Mul3 = RCC_PLL_MUL3,
		#endif // RCC_PLL_MUL3

		#ifdef RCC_PLL_MUL4
		Mul4 = RCC_PLL_MUL4,
		#endif // RCC_PLL_MUL4

		#ifdef RCC_PLL_MUL5
		Mul5 = RCC_PLL_MUL5,
		#endif // RCC_PLL_MUL5

		#ifdef RCC_PLL_MUL6
		Mul6 = RCC_PLL_MUL6,
		#endif // RCC_PLL_MUL6

		#ifdef RCC_PLL_MUL7
		Mul7 = RCC_PLL_MUL7,
		#endif // RCC_PLL_MUL7

		#ifdef RCC_PLL_MUL8
		Mul8 = RCC_PLL_MUL8,
		#endif // RCC_PLL_MUL8

		#ifdef RCC_PLL_MUL9
		Mul9 = RCC_PLL_MUL9,
		#endif // RCC_PLL_MUL9

		#ifdef RCC_PLL_MUL10
		Mul10 = RCC_PLL_MUL10,
		#endif // RCC_PLL_MUL10

		#ifdef RCC_PLL_MUL11
		Mul11 = RCC_PLL_MUL11,
		#endif // RCC_PLL_MUL11

		#ifdef RCC_PLL_MUL12
		Mul12 = RCC_PLL_MUL12,
		#endif // RCC_PLL_MUL12

		#ifdef RCC_PLL_MUL13
		Mul13 = RCC_PLL_MUL13,
		#endif // RCC_PLL_MUL13

		#ifdef RCC_PLL_MUL14
		Mul14 = RCC_PLL_MUL14,
		#endif // RCC_PLL_MUL14

		#ifdef RCC_PLL_MUL15
		Mul15 = RCC_PLL_MUL15,
		#endif // RCC_PLL_MUL15

		#ifdef RCC_PLL_MUL16
		Mul16 = RCC_PLL_MUL16,
		#endif // RCC_PLL_MUL16
	};

	/// <summary>
	///		PLL 初始化选项
	/// </summary>
	class PllInitOptions
	{
	public:
		PllInitOptions() {}

		PllInitOptions(RCC_PLLInitTypeDef const &def)
		{
			*this = def;
		}

		PllInitOptions &operator=(RCC_PLLInitTypeDef const &value);

		PllState _state = PllState::On;

		PllClockSource _clock_source = PllClockSource::HSE;

		/// <summary>
		///		倍频系数
		/// </summary>
		PllMul _mul = PllMul::Mul9;

		operator RCC_PLLInitTypeDef() const;
	};
}
