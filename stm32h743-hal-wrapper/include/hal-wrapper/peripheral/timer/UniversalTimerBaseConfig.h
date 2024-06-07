#pragma once
#include<base/HandleWrapper.h>
#include<hal.h>

namespace hal
{
	class UniversalTimerBaseConfig :
		public base::HandleWrapper<TIM_Base_InitTypeDef>
	{
	private:
		TIM_Base_InitTypeDef _handle { };

	public:
		TIM_Base_InitTypeDef &Handle() override;

		/// <summary>
		///     分频器。
		///     * 实际上就是一个计数器，这里是设置这个分频计数器的（进制数 - 1）。
		///		  例如设置为 0，这个分频计数器就是一个 0 + 1 = 1 进制计数器。
		///		  或者说这里设置的是分频计数器能达到的最大值。
		///		* 计数值等于（进制数 - 1）后，再加 1 就会触发进位和归 0。
		///     * 设置范围：[0x0000, 0xffff]
		///     * 设置为 0，则分频器每被输入一个时钟周期，就产生进位和清零。产生的效果就是不分频。
		///		* 输入给定时器计数器的时钟信号频率 = 分频器的输入信号 / (Prescaler + 1)
		/// </summary>
		/// <returns></returns>
		uint32_t Prescaler() const
		{
			return _handle.Prescaler;
		}
		void SetPrescaler(uint32_t value)
		{
			_handle.Prescaler = value;
		}

		enum class CounterModeOption
		{
			UP = TIM_COUNTERMODE_UP,
			DOWN = TIM_COUNTERMODE_DOWN,
			CENTERALIGNED1 = TIM_COUNTERMODE_CENTERALIGNED1,
			CENTERALIGNED2 = TIM_COUNTERMODE_CENTERALIGNED2,
			CENTERALIGNED3 = TIM_COUNTERMODE_CENTERALIGNED3,
		};

		/// <summary>
		///		计数模式
		/// </summary>
		/// <returns></returns>
		CounterModeOption CounterMode() const
		{
			return static_cast<CounterModeOption>(_handle.CounterMode);
		}
		void SetCounterMode(CounterModeOption value)
		{
			_handle.CounterMode = static_cast<uint32_t>(value);
		}

		/// <summary>
		///		定时周期。
		///		* 定时器从 0 开始计数，所以定时周期就是它的重装载值。
		///		* 设置范围：[0x0000, 0xffff]
		/// </summary>
		/// <returns></returns>
		uint32_t Period() const
		{
			return _handle.Period;
		}
		void SetPeriod(uint32_t value)
		{
			_handle.Period = value;
		}

		enum class ClockDivisionOption
		{
			DIV1 = TIM_CLOCKDIVISION_DIV1,
			DIV2 = TIM_CLOCKDIVISION_DIV2,
			DIV4 = TIM_CLOCKDIVISION_DIV4,
		};

		ClockDivisionOption ClockDivision() const
		{
			return static_cast<ClockDivisionOption>(_handle.ClockDivision);
		}
		void SetClockDivision(ClockDivisionOption value)
		{
			_handle.ClockDivision = static_cast<uint32_t>(value);
		}

		/// <summary>
		///		重复计数
		/// </summary>
		/// <returns></returns>
		uint32_t RepetitionCounter() const
		{
			return _handle.RepetitionCounter;
		}
		void SetRepetitionCounter(uint32_t value)
		{
			_handle.RepetitionCounter = value;
		}

		enum class AutoReloadPreloadOption
		{
			Disable = TIM_AUTORELOAD_PRELOAD_DISABLE,
			Enable = TIM_AUTORELOAD_PRELOAD_ENABLE,
		};

		/// <summary>
		///		自动重装载的预装载。
		/// </summary>
		/// <returns></returns>
		AutoReloadPreloadOption AutoReloadPreload() const
		{
			return static_cast<AutoReloadPreloadOption>(_handle.AutoReloadPreload);
		}
		void SetAutoReloadPreload(AutoReloadPreloadOption value)
		{
			_handle.AutoReloadPreload = static_cast<uint32_t>(value);
		}
	};
}