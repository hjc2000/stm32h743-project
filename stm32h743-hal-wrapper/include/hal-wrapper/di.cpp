#include <hal-wrapper/clock/SysTickClock.h>

bsp::ISysTick &DI_SysTick()
{
	return hal::SysTickClock::Instance();
}
