#include "GpioPinPH4.h"
#include <hal.h>

bsp::GpioPinPH4 &bsp::GpioPinPH4::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<GpioPinPH4>
	{
	public:
		std::unique_ptr<GpioPinPH4> Create() override
		{
			return std::unique_ptr<GpioPinPH4>{new GpioPinPH4{}};
		}
	};

	Getter o;
	return o.Instance();
}

GPIO_TypeDef *bsp::GpioPinPH4::Port()
{
	return GPIOH;
}

uint32_t bsp::GpioPinPH4::Pin()
{
	return GPIO_PIN_4;
}

std::string bsp::GpioPinPH4::PinName() const
{
	return "PH4";
}
