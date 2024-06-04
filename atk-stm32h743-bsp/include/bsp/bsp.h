#pragma once
#include<bsp-interface/IDelayer.h>
#include<bsp-interface/IDigitalLed.h>
#include<bsp-interface/IEventDrivenKey.h>
#include<bsp-interface/IKeyScanner.h>
#include<bsp-interface/ISerial.h>

enum class KeyIndex :uint16_t
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

	static bsp::IDigitalLed &RedDigitalLed();
	static bsp::IDigitalLed &GreenDigitalLed();

	static bsp::IDelayer &Delayer();

	static bsp::IKeyScanner &KeyScanner();
	static bsp::IEventDrivenKey &WakeUpKey();

	static bsp::ISerial &Serial();
};
