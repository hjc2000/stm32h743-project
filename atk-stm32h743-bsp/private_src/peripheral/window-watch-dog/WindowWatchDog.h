#pragma once
#include<Interrupt.h>
#include<WindowWatchDogInitOptions.h>
#include<functional>

namespace bsp
{
	using WindowWatchDogInitCallbackFunc = void(*)(WWDG_HandleTypeDef *handle);

	class WindowWatchDog
	{
		WWDG_HandleTypeDef _handle;
		bool IsClockEnabled();
		void EnableClock();
		void DisableClock();

	public:
		WWDG_HandleTypeDef *Handle();
		WWDG_TypeDef *HardwareInstance();

		WindowWatchDogInitCallbackFunc MspInitCallbackFunc();
		WindowWatchDogInitCallbackFunc EarlyWakeUpInterruptCallbackFunc();

		void Initialize(WindowWatchDogInitOptions const &options);

		void Feed()
		{
			HAL_WWDG_Refresh(Handle());
		}

		static WindowWatchDog &Instance()
		{
			static WindowWatchDog o;
			return o;
		}

		/// <summary>
		///		提早唤醒中断发生时触发的回调。
		///		* 不需要喂狗，因为本类的实现是先喂狗后再回调此函数的。
		/// </summary>
		std::function<void()> _on_early_wakeup_interrupt;
	};
}
