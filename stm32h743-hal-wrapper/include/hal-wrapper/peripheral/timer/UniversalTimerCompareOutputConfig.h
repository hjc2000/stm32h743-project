#pragma once
#include<base/HandleWrapper.h>
#include<hal.h>
#include<stdint.h>

namespace hal
{
	class UniversalTimerCompareOutputConfig :
		public base::HandleWrapper<TIM_OC_InitTypeDef>
	{
	private:
		TIM_OC_InitTypeDef _handle { };

	public:
		enum class OutputCompareModeOption
		{
			Timing = TIM_OCMODE_TIMING,
			Active = TIM_OCMODE_ACTIVE,
			Inactive = TIM_OCMODE_INACTIVE,
			Toggle = TIM_OCMODE_TOGGLE,
			Pwm1 = TIM_OCMODE_PWM1,
			Pwm2 = TIM_OCMODE_PWM2,
			ForcedActive = TIM_OCMODE_FORCED_ACTIVE,
			ForcedInactive = TIM_OCMODE_FORCED_INACTIVE,
			RetrigerrableOpm1 = TIM_OCMODE_RETRIGERRABLE_OPM1,
			RetrigerrableOpm2 = TIM_OCMODE_RETRIGERRABLE_OPM2,
			CombinedPwm1 = TIM_OCMODE_COMBINED_PWM1,
			CombinedPwm2 = TIM_OCMODE_COMBINED_PWM2,
			AsymmetricPwm1 = TIM_OCMODE_ASYMMETRIC_PWM1,
			AsymmetricPwm2 = TIM_OCMODE_ASYMMETRIC_PWM2,
		};

		OutputCompareModeOption OutputCompareMode() const
		{
			return static_cast<OutputCompareModeOption>(_handle.OCMode);
		}
		void SetOutputCompareMode(OutputCompareModeOption value)
		{
			_handle.OCMode = static_cast<uint32_t>(value);
		}

		/// <summary>
		///		范围：[0x0000, 0xffff]
		/// </summary>
		uint32_t Pulse()
		{
			return _handle.Pulse;
		}
		void SetPulse(uint32_t value)
		{
			_handle.Pulse = value;
		}

		uint32_t OCPolarity;    /*!< Specifies the output polarity.
		This parameter can be a value of @ref TIM_Output_Compare_Polarity */

		uint32_t OCNPolarity;   /*!< Specifies the complementary output polarity.
		This parameter can be a value of @ref TIM_Output_Compare_N_Polarity
		@note This parameter is valid only for timer instances supporting break feature. */

		uint32_t OCFastMode;    /*!< Specifies the Fast mode state.
		This parameter can be a value of @ref TIM_Output_Fast_State
		@note This parameter is valid only in PWM1 and PWM2 mode. */


		uint32_t OCIdleState;   /*!< Specifies the TIM Output Compare pin state
		during Idle state.
		This parameter can be a value of @ref TIM_Output_Compare_Idle_State
		@note This parameter is valid only for timer instances supporting break feature. */

		uint32_t OCNIdleState;  /*!< Specifies the TIM Output Compare pin state during
		Idle state.
		This parameter can be a value of @ref TIM_Output_Compare_N_Idle_State
		@note This parameter is valid only for timer instances supporting break feature. */
	};
}
