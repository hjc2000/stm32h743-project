#include"Cache.h"
#include<hal.h>

void hal::Cache::Enable()
{
	SCB_EnableICache();
	SCB_EnableDCache();
	SCB->CACR |= 1 << 2;
}

void hal::Cache::Disable()
{
	SCB_DisableICache();
	SCB_DisableDCache();
}
