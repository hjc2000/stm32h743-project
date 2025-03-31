#include "bsp-interface/di/interrupt.h"
#include "hal.h"

extern "C"
{

	/* #region 定时器中断 */

	void TIM3_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::TIM3_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	/* #endregion */

	void WWDG1_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::WWDG_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	void USART1_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::USART1_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	/* #region DMA */

	void DMA_STR0_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	void DMA_STR1_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	/* #endregion */

	/* #region EXTI */

	void EXTI0_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	void EXTI1_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	void EXTI2_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	void EXTI3_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	void EXTI4_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	/* #endregion */

	///
	/// @brief 以太网中断服务函数。
	/// @param
	///
	void ETH_IRQHandler(void)
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::ETH_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}
}
