#include <bsp-interface/di/independent-watch-dog.h>
#include <IndependentWatchDog.h>

bsp::IIndependentWatchDog &DI_IndependentWatchDog()
{
    return bsp::IndependentWatchDog::Instance();
}
