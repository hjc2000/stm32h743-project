#pragma once
#include<bsp-interface/IDigitalLed.h>

class BSP
{
private:
	BSP() = delete;

public:
	static void Initialize();

	static bsp::IDigitalLed &RedLed();
};
