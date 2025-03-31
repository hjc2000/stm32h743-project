#include <bsp-interface/di/systick.h>
#include <SysTickClock.h>

bsp::ISysTick &DI_SysTick()
{
    return bsp::SysTickClock::Instance();
}
