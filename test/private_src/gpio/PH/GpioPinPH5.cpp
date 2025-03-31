#include "GpioPinPH5.h"
#include <hal.h>

bsp::GpioPinPH5 &bsp::GpioPinPH5::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<GpioPinPH5>
	{
	public:
		std::unique_ptr<GpioPinPH5> Create() override
		{
			return std::unique_ptr<GpioPinPH5>{new GpioPinPH5{}};
		}
	};

	Getter o;
	return o.Instance();
}

GPIO_TypeDef *bsp::GpioPinPH5::Port()
{
	return GPIOH;
}

uint32_t bsp::GpioPinPH5::Pin()
{
	return GPIO_PIN_5;
}

std::string bsp::GpioPinPH5::PinName() const
{
	return "PH5";
}
