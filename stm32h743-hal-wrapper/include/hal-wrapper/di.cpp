#include <hal-wrapper/clock/SysTickClock.h>
#include <task/di.h>

namespace task
{
	bsp::ISysTick &DI_SysTick()
	{
		return hal::SysTickClock::Instance();
	}
}
