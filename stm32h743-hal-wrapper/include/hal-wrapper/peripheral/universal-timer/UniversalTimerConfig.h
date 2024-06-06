#pragma once
#include<base/HandleWrapper.h>
#include<hal.h>

namespace hal
{
	class UniversalTimerConfig :
		public base::HandleWrapper<TIM_Base_InitTypeDef>
	{
	private:
		TIM_Base_InitTypeDef _handle { };

	public:
		UniversalTimerConfig() = default;
		UniversalTimerConfig(UniversalTimerConfig const &o);
		UniversalTimerConfig &operator=(UniversalTimerConfig const &o);

		TIM_Base_InitTypeDef &Handle() override;

		/// <summary>
		///     分频器。
		///     * 实际上就是一个计数器，这里是设置这个计数器的最大计数值。
		///       等于计数值后再加 1 就会清零并产生进位。
		///     * 设置范围：[0x0000, 0xffff]
		///     * 设置为 0，则分频器每被输入一个时钟周期，就产生进位和清零。产生的效果就是不分频。
		/// </summary>
		/// <returns></returns>
		uint32_t Prescaler()
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
		CounterModeOption CounterMode()
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
		uint32_t Period()
		{
			return _handle.Period;
		}
		void SetPeriod(uint32_t value)
		{
			_handle.Period = value;
		}

		#define TIM_CLOCKDIVISION_DIV1             0x00000000U                          /*!< Clock division: tDTS=tCK_INT   */
		#define TIM_CLOCKDIVISION_DIV2             TIM_CR1_CKD_0                        /*!< Clock division: tDTS=2*tCK_INT */
		#define TIM_CLOCKDIVISION_DIV4             TIM_CR1_CKD_1                        /*!< Clock division: tDTS=4*tCK_INT */

		enum class ClockDivisionOption
		{
			DIV1,
			DIV2,
			DIV3,
		};

		ClockDivisionOption ClockDivision()
		{
			return static_cast<ClockDivisionOption>(_handle.ClockDivision);
		}
		void SetClockDivision(ClockDivisionOption value)
		{
			_handle.ClockDivision = static_cast<uint32_t>(value);
		}

		uint32_t RepetitionCounter;  /*!< Specifies the repetition counter value.
		Each time the RCR downcounter
		reaches zero, an update event is generated and counting restarts
		from the RCR value (N).
		This means in PWM mode that (N+1) corresponds to:
		- the number of PWM periods in edge-aligned mode
		- the number of half PWM period in center-aligned mode
		GP timers: this parameter must be a number between Min_Data = 0x00 and
		Max_Data = 0xFF.
		Advanced timers: this parameter must be a number between Min_Data = 0x0000 and
		Max_Data = 0xFFFF. */

		uint32_t AutoReloadPreload;  /*!< Specifies the auto-reload preload.
		This parameter can be a value of @ref TIM_AutoReloadPreload */

	};
}