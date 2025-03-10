#pragma once
#include <base/define.h>
#include <bsp-interface/IDigitalLed.h>
#include <bsp-interface/key/IEventDrivenKey.h>
#include <bsp-interface/key/IKeyScanner.h>
#include <bsp-interface/serial/ISerial.h>
#include <bsp-interface/timer/IDelayer.h>

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
