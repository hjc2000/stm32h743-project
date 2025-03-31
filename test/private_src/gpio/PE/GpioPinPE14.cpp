#include "GpioPinPE14.h"
#include <hal.h>

bsp::GpioPinPE14 &bsp::GpioPinPE14::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<GpioPinPE14>
    {
    public:
        std::unique_ptr<GpioPinPE14> Create() override
        {
            return std::unique_ptr<GpioPinPE14>{new GpioPinPE14{}};
        }
    };

    Getter o;
    return o.Instance();
}

GPIO_TypeDef *bsp::GpioPinPE14::Port()
{
    return GPIOE;
}

uint32_t bsp::GpioPinPE14::Pin()
{
    return GPIO_PIN_14;
}

std::string bsp::GpioPinPE14::PinName() const
{
    return "PE14";
}

void bsp::GpioPinPE14::OpenAsAlternateFunctionMode(std::string function_name, bsp::IGpioPinPullMode pull_mode, bsp::IGpioPinDriver driver_mode)
{
    if (_is_open)
    {
        throw std::runtime_error{PinName() + " 已经打开"};
    }

    EnableClock();
    GPIO_InitTypeDef def{};
    if (function_name == "fmc")
    {
        def.Alternate = GPIO_AF12_FMC;
    }
    else
    {
        throw std::invalid_argument{"不支持的 AlternateFunction"};
    }

    switch (pull_mode)
    {
    default:
    case bsp::IGpioPinPullMode::NoPull:
        {
            def.Pull = GPIO_NOPULL;
            break;
        }
    case bsp::IGpioPinPullMode::PullUp:
        {
            def.Pull = GPIO_PULLUP;
            break;
        }
    case bsp::IGpioPinPullMode::PullDown:
        {
            def.Pull = GPIO_PULLDOWN;
            break;
        }
    }

    switch (driver_mode)
    {
    case bsp::IGpioPinDriver::PushPull:
        {
            def.Mode = GPIO_MODE_AF_PP;
            break;
        }
    case bsp::IGpioPinDriver::OpenDrain:
        {
            def.Mode = GPIO_MODE_AF_OD;
            break;
        }
    default:
        {
            throw std::invalid_argument{"不支持的 Driver"};
        }
    }

    def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    def.Pin = Pin();
    HAL_GPIO_Init(Port(), &def);
    _is_open = true;
}
