#include "base/define.h"
#include "base/peripheral/IDigitalLed.h"
#include "GreenDigitalLed.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/interrupt.h>
#include <RedDigitalLed.h>

namespace
{
	class DictionaryProvider
	{
	public:
		DictionaryProvider()
		{
			_dic.Add("red_led", &bsp::RedDigitalLed::Instance());
			_dic.Add("green_led", &bsp::GreenDigitalLed::Instance());
		}

		base::Dictionary<std::string, base::led::IDigitalLed *> _dic;
	};

} // namespace

PREINIT(base::led::DigitalLedCollection)

base::IDictionary<std::string, base::led::IDigitalLed *> const &base::led::DigitalLedCollection()
{
	static DictionaryProvider o;
	return o._dic;
}

base::led::IDigitalLed &base::led::RedDigitalLed()
{
	return bsp::RedDigitalLed::Instance();
}

base::led::IDigitalLed &base::led::GreenDigitalLed()
{
	return bsp::GreenDigitalLed::Instance();
}
