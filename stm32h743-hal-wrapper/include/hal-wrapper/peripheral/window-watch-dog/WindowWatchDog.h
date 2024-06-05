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
