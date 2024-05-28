#include"Cache.h"
#include<hal.h>

void bsp::Cache::Enable()
{
	SCB_EnableICache();
	SCB_EnableDCache();
	SCB->CACR |= 1 << 2;
}

void bsp::Cache::Disable()
{
	SCB_DisableICache();
	SCB_DisableDCache();
}
