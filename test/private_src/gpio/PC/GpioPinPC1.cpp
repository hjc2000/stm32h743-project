#include "GpioPinPC1.h"
#include <hal.h>

bsp::GpioPinPC1 &bsp::GpioPinPC1::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<GpioPinPC1>
    {
    public:
        std::unique_ptr<GpioPinPC1> Create() override
        {
            return std::unique_ptr<GpioPinPC1>{new GpioPinPC1{}};
        }
    };

    Getter o;
    return o.Instance();
}

GPIO_TypeDef *bsp::GpioPinPC1::Port()
{
    return GPIOC;
}

uint32_t bsp::GpioPinPC1::Pin()
{
    return GPIO_PIN_1;
}

std::string bsp::GpioPinPC1::PinName() const
{
    return "PC1";
}
