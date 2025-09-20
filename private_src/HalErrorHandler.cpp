#include "base/embedded/led/led_handle.h"
#include "hal.h"

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
		base::led::turn_on_error_led();

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
		base::led::turn_on_error_led();

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
		base::led::turn_on_error_led();

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
		base::led::turn_on_error_led();

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

	extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

	/**
	 * @brief This function handles USB On The Go FS End Point 1 Out global interrupt.
	 */
	void OTG_FS_EP1_OUT_IRQHandler(void)
	{
		/* USER CODE BEGIN OTG_FS_EP1_OUT_IRQn 0 */

		/* USER CODE END OTG_FS_EP1_OUT_IRQn 0 */
		HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
		/* USER CODE BEGIN OTG_FS_EP1_OUT_IRQn 1 */

		/* USER CODE END OTG_FS_EP1_OUT_IRQn 1 */
	}

	/**
	 * @brief This function handles USB On The Go FS End Point 1 In global interrupt.
	 */
	void OTG_FS_EP1_IN_IRQHandler(void)
	{
		/* USER CODE BEGIN OTG_FS_EP1_IN_IRQn 0 */

		/* USER CODE END OTG_FS_EP1_IN_IRQn 0 */
		HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
		/* USER CODE BEGIN OTG_FS_EP1_IN_IRQn 1 */

		/* USER CODE END OTG_FS_EP1_IN_IRQn 1 */
	}

	void OTG_FS_IRQHandler(void)
	{
		/* USER CODE BEGIN OTG_FS_IRQn 0 */

		/* USER CODE END OTG_FS_IRQn 0 */
		HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
		/* USER CODE BEGIN OTG_FS_IRQn 1 */

		/* USER CODE END OTG_FS_IRQn 1 */
	}
}
