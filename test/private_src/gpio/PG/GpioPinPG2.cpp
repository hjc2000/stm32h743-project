#include "GpioPinPG2.h"
#include <hal.h>

bsp::GpioPinPG2 &bsp::GpioPinPG2::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<GpioPinPG2>
    {
    public:
        std::unique_ptr<GpioPinPG2> Create() override
        {
            return std::unique_ptr<GpioPinPG2>{new GpioPinPG2{}};
        }
    };

    Getter o;
    return o.Instance();
}

GPIO_TypeDef *bsp::GpioPinPG2::Port()
{
    return GPIOG;
}

uint32_t bsp::GpioPinPG2::Pin()
{
    return GPIO_PIN_2;
}

std::string bsp::GpioPinPG2::PinName() const
{
    return "PG2";
}

void bsp::GpioPinPG2::OpenAsAlternateFunctionMode(std::string function_name, bsp::IGpioPinPullMode pull_mode, bsp::IGpioPinDriver driver_mode)
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
