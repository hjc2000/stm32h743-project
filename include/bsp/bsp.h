#pragma once
#include "base/task/delay.h"
#include <base/define.h>

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
