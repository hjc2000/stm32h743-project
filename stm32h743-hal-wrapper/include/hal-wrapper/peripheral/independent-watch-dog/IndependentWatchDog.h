#pragma once
#include<base/HandleWrapper.h>
#include<bsp-interface/IIndependentWatchDog.h>
#include<chrono>
#include<hal-wrapper/peripheral/independent-watch-dog/IndependentWatchDogConfig.h>

namespace hal
{
	/// <summary>
	///		独立看门狗。
	///		- 所谓独立看门狗就是具有自己的内部时钟源，不依赖单片机的系统时钟。
	///		  在系统时钟失效时仍然能工作。
	/// </summary>
	class IndependentWatchDog :
		public bsp::IIndependentWatchDog,
		public base::HandleWrapper<IWDG_HandleTypeDef>
	{
	private:
		IWDG_HandleTypeDef _handle;
		IndependentWatchDogConfig _config;

		/// <summary>
		///		内部时钟信号的频率。还要经过预分频才会输入到计数器。
		/// </summary>
		/// <returns></returns>
		uint32_t InnerClockSourceFreq_Hz() const
		{
			// 独立看门狗具有 40 kHz 的内部时钟。
			return 40 * 1000;
		}

		void Initialize();

	public:
		static IndependentWatchDog &Instance()
		{
			static IndependentWatchDog o;
			return o;
		}

		IWDG_HandleTypeDef &Handle() override
		{
			return _handle;
		}

		IWDG_TypeDef *HardwareInstance()
		{
			return IWDG1;
		}

		std::chrono::milliseconds WatchDogTimeoutDuration() const override;
		void SetWatchDogTimeoutDuration(std::chrono::milliseconds value) override;
		void Feed() override;
	};
}