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

		enum class OutputComparePolarityOption
		{
			High = TIM_OCPOLARITY_HIGH,
			Low = TIM_OCPOLARITY_LOW,
		};

		OutputComparePolarityOption OutputComparePolarity() const
		{
			return static_cast<OutputComparePolarityOption>(_handle.OCPolarity);
		}
		void SetOutputComparePolarity(OutputComparePolarityOption value)
		{
			_handle.OCPolarity = static_cast<uint32_t>(value);
		}

		enum class OutputCompare_N_PolarityOption
		{
			High = TIM_OCNPOLARITY_HIGH,
			Low = TIM_OCNPOLARITY_LOW,
		};

		OutputCompare_N_PolarityOption OutputCompare_N_Polarity() const
		{
			return static_cast<OutputCompare_N_PolarityOption>(_handle.OCNPolarity);
		}
		void SetOutputCompare_N_Polarity(OutputCompare_N_PolarityOption value)
		{
			_handle.OCNPolarity = static_cast<uint32_t>(value);
		}

		enum class OutputCompareFastModeOption
		{
			Disable = TIM_OCFAST_DISABLE,
			Enable = TIM_OCFAST_ENABLE,
		};

		OutputCompareFastModeOption OutputCompareFastMode() const
		{
			return static_cast<OutputCompareFastModeOption>(_handle.OCFastMode);
		}
		void SetOutputCompareFastMode(OutputCompareFastModeOption value)
		{
			_handle.OCFastMode = static_cast<uint32_t>(value);
		}

		enum class OutputCompareIdleStateOption
		{
			Set = TIM_OCIDLESTATE_SET,
			Reset = TIM_OCIDLESTATE_RESET,
		};

		OutputCompareIdleStateOption OutputCompareIdleState() const
		{
			return static_cast<OutputCompareIdleStateOption>(_handle.OCIdleState);
		}
		void SetOutputCompareIdleState(OutputCompareIdleStateOption value)
		{
			_handle.OCIdleState = static_cast<uint32_t>(value);
		}

		enum class OutputCompare_N_IdleStateOption
		{
			Set = TIM_OCNIDLESTATE_SET,
			Reset = TIM_OCNIDLESTATE_RESET,
		};

		OutputCompare_N_IdleStateOption OutputCompare_N_IdleState() const
		{
			return static_cast<OutputCompare_N_IdleStateOption>(_handle.OCNIdleState);
		}
		void SetOutputCompare_N_IdleState(OutputCompare_N_IdleStateOption value)
		{
			_handle.OCNIdleState = static_cast<uint32_t>(value);
		}
	};
}
