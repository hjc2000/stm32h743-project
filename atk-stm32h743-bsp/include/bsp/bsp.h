#pragma once
#include <base/define.h>
#include <bsp-interface/IDigitalLed.h>
#include <bsp-interface/interrupt/IInterruptSwitch.h>
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

class BSP
{
private:
    BSP() = delete;

public:
    static_function void Initialize();
};

void TestWindowWatchDog();
void TestUniversalTimer1();
