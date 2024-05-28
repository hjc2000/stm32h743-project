#include"WindowWatchDog.h"

using namespace bsp;

WWDG_HandleTypeDef *bsp::WindowWatchDog::Handle()
{
	return &_handle;
}

WWDG_TypeDef *bsp::WindowWatchDog::HardwareInstance()
{
	return WWDG1;
}

WindowWatchDogInitCallbackFunc bsp::WindowWatchDog::MspInitCallbackFunc()
{
	return [](WWDG_HandleTypeDef *handle)->void
	{
		WindowWatchDog::Instance().EnableClock();
		Interrupt::SetPriority(IRQn_Type::WWDG_IRQn, 2, 3);
		Interrupt::EnableIRQ(IRQn_Type::WWDG_IRQn);
	};
}

WindowWatchDogInitCallbackFunc bsp::WindowWatchDog::EarlyWakeUpInterruptCallbackFunc()
{
	return [](WWDG_HandleTypeDef *handle)->void
	{
		WindowWatchDog::Instance().Feed();
		if (WindowWatchDog::Instance()._on_early_wakeup_interrupt)
		{
			WindowWatchDog::Instance()._on_early_wakeup_interrupt();
		}
	};
}

void bsp::WindowWatchDog::Initialize(WindowWatchDogInitOptions const &options)
{
	Handle()->Instance = HardwareInstance();
	Handle()->Init = options;
	Handle()->MspInitCallback = MspInitCallbackFunc();
	Handle()->EwiCallback = EarlyWakeUpInterruptCallbackFunc();
	HAL_WWDG_Init(Handle());
}

bool bsp::WindowWatchDog::IsClockEnabled()
{
	return __HAL_RCC_WWDG_IS_CLK_ENABLED();
}

void bsp::WindowWatchDog::EnableClock()
{
	__HAL_RCC_WWDG_CLK_ENABLE();
}

void bsp::WindowWatchDog::DisableClock()
{
	__HAL_RCC_WWDG_CLK_DISABLE();
}

extern "C"
{
	void WWDG_IRQHandler()
	{
		HAL_WWDG_IRQHandler(WindowWatchDog::Instance().Handle());
	}
}
