#pragma once
#include<bsp-interface/IIndependentWatchDog.h>
#include<chrono>
#include<IndependentWatchDogInitOptions.h>

namespace bsp
{
	/// <summary>
	///		独立看门狗。
	///		- 所谓独立看门狗就是具有自己的内部时钟源，不依赖单片机的系统时钟。
	///		  在系统时钟失效时仍然能工作。
	/// </summary>
	class IndependentWatchDog :
		public bsp::IIndependentWatchDog
	{
	private:
		IWDG_HandleTypeDef _handle;
		void Initialize(IndependentWatchDogInitOptions const &options);

	public:
		static IndependentWatchDog &Instance()
		{
			static IndependentWatchDog o;
			return o;
		}

		IWDG_HandleTypeDef *Handle()
		{
			return &_handle;
		}

		IWDG_TypeDef *HardwareInstance()
		{
			return IWDG1;
		}

		/// <summary>
		///		分频系数。
		/// </summary>
		/// <returns></returns>
		IndependentWatchDogPrescaler Prescaler()
		{
			return IndependentWatchDogPrescaler(Handle()->Init.Prescaler);
		}

		uint32_t PrescalerValue();

		/// <summary>
		///		内部时钟信号的频率。还要经过预分频才会输入到计数器。
		/// </summary>
		/// <returns></returns>
		uint32_t InnerClockSourceFreq_Hz()
		{
			// 独立看门狗具有 40 kHz 的内部时钟。
			return 40 * 1000;
		}

		#pragma region IWatchDog
		std::chrono::milliseconds WatchDogTimeoutDuration() override;
		void SetWatchDogTimeoutDuration(std::chrono::milliseconds value) override;
		void Feed() override;
		#pragma endregion

	};
}