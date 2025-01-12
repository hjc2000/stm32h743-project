#include <base/container/Dictionary.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/led.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <DigitalLed.h>

namespace
{
	class Initializer
	{
	public:
		Initializer()
		{
			_dic.Add("red_led", &bsp::RedDigitalLed::Instance());
			_dic.Add("green_led", &bsp::GreenDigitalLed::Instance());
		}

		base::Dictionary<std::string, bsp::IDigitalLed *> _dic;
	};

	/// @brief 单例获取器
	class Getter :
		public bsp::TaskSingletonGetter<Initializer>
	{
	public:
		std::unique_ptr<Initializer> Create() override
		{
			return std::unique_ptr<Initializer>{new Initializer{}};
		}
	};
} // namespace

/// @brief 数字 LED 灯集合。
/// @return
base::IDictionary<std::string, bsp::IDigitalLed *> const &DI_DigitalLedCollection()
{
	Getter g;
	return g.Instance()._dic;
}

bsp::IDigitalLed &DI_RedDigitalLed()
{
	return bsp::RedDigitalLed::Instance();
}

bsp::IDigitalLed &DI_GreenDigitalLed()
{
	return bsp::GreenDigitalLed::Instance();
}
