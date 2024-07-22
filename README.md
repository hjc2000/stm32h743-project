# stm32h743-project

# 关于局部静态变量

C++ 的局部 static 变量在第一次执行到那个作用域时会初始化，执行构造函数。如果不注意，在 static 变量执行构造函数的时候，另一个 freertso 任务也去访问同一个 static 变量，就会导致意外。所以需要遵守一些原则。

## static 变量与单例模式

用到 static 变量一定是因为单例模式。通过内部 static 变量返回单例的函数，我们把它叫做 `单例函数` 。

* 例如本项目中的 class 经常可见一个 static 的 Instance 函数返回本 class 的单例。
* 或者有一个函数叫 GetKeyScanner 会返回一个单例的 KeyScanner 对象。

## 单例之间的依赖链

一个单例可能依赖其他单例，例如 KeyScanner 单例会依赖一些 IKey 的单例。当这种情况发生时，所有的初始化工作都应该在使用者（在这里是 KeyScanner），的构造函数中完成。



例如 KeyScanner 需要在自己的构造函数中初始化完 IKey 的列表，将这些按键单例的指针放到列表中。在这个过程中，就会调用用来获取 IKey 单例的单例函数，例如 `Key1::Instance` 。这就会触发其中的静态局部变量的初始化，从而执行 Key1 的构造函数。就这样，顺着依赖链，所有 static 变量的构造函数最终在最上层的一个单例函数中触发执行。在本项目中就是 BSP 类的那些函数。



BSP 类没有构造函数，只有一些静态函数。如下所示：

```cpp
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
```

这些静态函数就是最顶层的单例函数，要让其中的 static 变量初始化，必须先调用一次，而且只能在单线程中调用，此时还不能启动 freertos。



所以需要一个 InitializeInstance 函数。如下所示

```cpp
void BSP::InitializeInstance()
{
	RedDigitalLed();
	GreenDigitalLed();

	KeyScanner();
	WakeUpKey();

	Serial();

	IndependentWatchDog();

	InterruptSwitch();
}

```

这个函数用来调用所有最上层的单例函数，从而顺着依赖链，调用所有单例函数，从而使所有局部 static 变量的构造函数得到执行。