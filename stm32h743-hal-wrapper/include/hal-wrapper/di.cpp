#include <bsp-interface/di.h>
#include <hal-wrapper/clock/SysTickClock.h>
#include <task/TaskDelayer.h>
#include <task/di.h>

namespace task
{
	bsp::ISysTick &DI_SysTick()
	{
		return hal::SysTickClock::Instance();
	}
}

namespace bsp
{
	bsp::IDelayer &DI_Delayer()
	{
		return task::TaskDelayer::Instance();
	}
}
