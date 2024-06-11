#pragma once
#include<bsp-interface/timer/ITimerInterruptManager.h>

namespace hal
{
	class TimerInterruptManager :
		public bsp::ITimerInterruptManager
	{
	private:
		std::map<uint32_t, std::function<void()>> _handler_map;

	public:
		static TimerInterruptManager &Instance()
		{
			static TimerInterruptManager o;
			return o;
		}

		std::map<uint32_t, std::function<void()>> &HandleMap() override;
		void DisableTimerInterrupt(uint32_t timer_id) noexcept override;
		void EnableTimerInterrupt(uint32_t timer_id) noexcept override;
	};
}