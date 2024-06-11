#pragma once
#include<bsp-interface/interrupt/IsrManager.h>

namespace hal
{
	bsp::IsrManager &GetIsrManager();
}