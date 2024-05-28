#pragma once
#include<hal.h>

namespace bsp
{
	/// <summary>
	///		PLL 初始化选项
	/// </summary>
	class PllConfig
	{
	public:
        uint32_t PLLState;   /*!< The new state of the PLL.
        This parameter can be a value of @ref RCC_PLL_Config                      */

        uint32_t PLLSource;  /*!< RCC_PLLSource: PLL entry clock source.
        This parameter must be a value of @ref RCC_PLL_Clock_Source               */

        uint32_t PLLM;       /*!< PLLM: Division factor for PLL VCO input clock.
        This parameter must be a number between Min_Data = 1 and Max_Data = 63    */

        uint32_t PLLN;       /*!< PLLN: Multiplication factor for PLL VCO output clock.
        This parameter must be a number between Min_Data = 4 and Max_Data = 512
        or between Min_Data = 8 and Max_Data = 420(*)
        (*) : For stm32h7a3xx and stm32h7b3xx family lines.                       */

        uint32_t PLLP;       /*!< PLLP: Division factor for system clock.
        This parameter must be a number between Min_Data = 2 and Max_Data = 128
        odd division factors are not allowed                                        */

        uint32_t PLLQ;       /*!< PLLQ: Division factor for peripheral clocks.
        This parameter must be a number between Min_Data = 1 and Max_Data = 128   */

        uint32_t PLLR;       /*!< PLLR: Division factor for peripheral clocks.
        This parameter must be a number between Min_Data = 1 and Max_Data = 128   */
        uint32_t PLLRGE;     /*!<PLLRGE: PLL1 clock Input range
        This parameter must be a value of @ref RCC_PLL1_VCI_Range                    */
        uint32_t PLLVCOSEL;  /*!<PLLVCOSEL: PLL1 clock Output range
        This parameter must be a value of @ref RCC_PLL1_VCO_Range                    */

        uint32_t PLLFRACN;   /*!<PLLFRACN: Specifies Fractional Part Of The Multiplication Factor for
        PLL1 VCO It should be a value between 0 and 8191                              */

    };
}
