#pragma once
#include<functional>
#include<hal-wrapper/interrupt/Interrupt.h>
#include<hal-wrapper/peripheral/window-watch-dog/WindowWatchDogConfig.h>

namespace hal
{
	class WindowWatchDog :
		public base::HandleWrapper<WWDG_HandleTypeDef>
	{
	private:
		WWDG_HandleTypeDef _handle;

		/// <summary>
		///		提早唤醒中断发生时触发的回调。
		///		* 不需要喂狗，因为本类的实现是先喂狗后再回调此函数的。
		/// </summary>
		std::function<void()> _on_early_wakeup_interrupt;

		static void OnMspInitCallback(WWDG_HandleTypeDef *handle);
		static void OnEarlyWakeUpInterruptCallback(WWDG_HandleTypeDef *handle);

	public:
		static WindowWatchDog &Instance()
		{
			static WindowWatchDog o;
			return o;
		}

		WWDG_HandleTypeDef &Handle() override;
		WWDG_TypeDef *HardwareInstance();

		void Initialize(WindowWatchDogConfig &options);
		void Feed();
	};
}
