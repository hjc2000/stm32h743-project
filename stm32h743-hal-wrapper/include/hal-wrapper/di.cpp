#include <hal-wrapper/clock/SysTickClock.h>
#include <task/dependencies.h>

bsp::ISysTick &task::DI_SysTick()
{
	return hal::SysTickClock::Instance();
}
