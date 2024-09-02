#pragma once
#include <bsp-interface/IDelayer.h>
#include <bsp-interface/IDigitalLed.h>
#include <bsp-interface/IIndependentWatchDog.h>
#include <bsp-interface/interrupt/IInterruptSwitch.h>
#include <bsp-interface/key/IEventDrivenKey.h>
#include <bsp-interface/key/IKeyScanner.h>
#include <bsp-interface/serial/ISerial.h>

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
    static void Initialize();
};

void TestWindowWatchDog();
void TestUniversalTimer1();
void TestFlash();
void TestGpio();
