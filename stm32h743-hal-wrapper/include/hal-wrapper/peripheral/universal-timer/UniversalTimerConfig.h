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

        uint32_t CounterMode;       /*!< Specifies the counter mode.
        This parameter can be a value of @ref TIM_Counter_Mode */

        uint32_t Period;            /*!< Specifies the period value to be loaded into the active
        Auto-Reload Register at the next update event.
        This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF.  */

        uint32_t ClockDivision;     /*!< Specifies the clock division.
        This parameter can be a value of @ref TIM_ClockDivision */

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