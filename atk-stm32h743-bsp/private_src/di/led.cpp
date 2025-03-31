#include "base/peripheral/IDigitalLed.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/interrupt.h>
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

		base::Dictionary<std::string, base::led::IDigitalLed *> _dic;
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
base::IDictionary<std::string, base::led::IDigitalLed *> const &base::led::DigitalLedCollection()
{
	Getter g;
	return g.Instance()._dic;
}

base::led::IDigitalLed &base::led::RedDigitalLed()
{
	return bsp::RedDigitalLed::Instance();
}

base::led::IDigitalLed &base::led::GreenDigitalLed()
{
	return bsp::GreenDigitalLed::Instance();
}
