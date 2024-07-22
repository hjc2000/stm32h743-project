#pragma once
#include <bsp-interface/IDelayer.h>
#include <bsp-interface/IDigitalLed.h>
#include <bsp-interface/IEventDrivenKey.h>
#include <bsp-interface/IIndependentWatchDog.h>
#include <bsp-interface/ISerial.h>
#include <bsp-interface/interrupt/IInterruptSwitch.h>
#include <bsp-interface/key/IKeyScanner.h>

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

	/// @brief 将 BSP 类中返回单例引用的静态函数全部调用一遍，触发局部 static 变量的初始化。
	/// 在单线程中，即 freertos 还没启动时执行一次本函数，进行初始化，可以防止 static 变量
	/// 初始化发生竟态。
	/// @note 本函数会在 Initialize 函数中被执行。
	static void InitializeInstance();

public:
	static void Initialize();

	static bsp::IDigitalLed &RedDigitalLed();
	static bsp::IDigitalLed &GreenDigitalLed();

	static bsp::IKeyScanner &KeyScanner();
	static bsp::IEventDrivenKey &WakeUpKey();

	static bsp::ISerial &Serial();

	/// @brief 独立看门狗。
	/// @return
	static bsp::IIndependentWatchDog &IndependentWatchDog();

	static bsp::IInterruptSwitch &InterruptSwitch();
};

void TestWindowWatchDog();
void TestUniversalTimer1();
void TestFlash();
void TestGpio();
