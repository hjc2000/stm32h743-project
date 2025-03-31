#include "GpioPinPB12.h"
#include <hal.h>

bsp::GpioPinPB12 &bsp::GpioPinPB12::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<GpioPinPB12>
	{
	public:
		std::unique_ptr<GpioPinPB12> Create() override
		{
			return std::unique_ptr<GpioPinPB12>{new GpioPinPB12{}};
		}
	};

	Getter o;
	return o.Instance();
}

GPIO_TypeDef *bsp::GpioPinPB12::Port()
{
	return GPIOB;
}

uint32_t bsp::GpioPinPB12::Pin()
{
	return GPIO_PIN_12;
}

std::string bsp::GpioPinPB12::PinName() const
{
	return "PB12";
}
