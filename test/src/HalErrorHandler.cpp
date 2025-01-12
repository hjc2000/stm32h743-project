#include <bsp-interface/di/led.h>

extern "C"
{
	/**
	 * @brief  This function handles NMI exception.
	 * @param  None
	 * @retval None
	 */
	void NMI_Handler()
	{
	}

	/**
	 * @brief  This function handles Hard Fault exception.
	 * @param  None
	 * @retval None
	 */
	void HardFault_Handler()
	{
		DI_RedDigitalLed().TurnOn();

		/* Go to infinite loop when Hard Fault exception occurs */
		while (1)
		{
		}
	}

	/**
	 * @brief  This function handles Memory Manage exception.
	 * @param  None
	 * @retval None
	 */
	void MemManage_Handler()
	{
		DI_RedDigitalLed().TurnOn();

		/* Go to infinite loop when Memory Manage exception occurs */
		while (1)
		{
		}
	}

	/**
	 * @brief  This function handles Bus Fault exception.
	 * @param  None
	 * @retval None
	 */
	void BusFault_Handler()
	{
		DI_RedDigitalLed().TurnOn();

		/* Go to infinite loop when Bus Fault exception occurs */
		while (1)
		{
		}
	}

	/**
	 * @brief  This function handles Usage Fault exception.
	 * @param  None
	 * @retval None
	 */
	void UsageFault_Handler()
	{
		DI_RedDigitalLed().TurnOn();

		/* Go to infinite loop when Usage Fault exception occurs */
		while (1)
		{
		}
	}

	/**
	 * @brief  This function handles Debug Monitor exception.
	 * @param  None
	 * @retval None
	 */
	void DebugMon_Handler()
	{
	}
}
