#pragma once
#include "base/peripheral/led/IDigitalLed.h"
#include "base/task/delay.h"
#include <base/define.h>
#include <bsp-interface/key/IEventDrivenKey.h>
#include <bsp-interface/key/IKeyScanner.h>

enum class KeyIndex : uint16_t
{
	Key0,
	Key1,
	Key2,
	KeyWakeUp,
	EnumEndFlag,
};

void TestWindowWatchDog();
void TestUniversalTimer1();
void TestSDRAM();
void TestEthernet();
