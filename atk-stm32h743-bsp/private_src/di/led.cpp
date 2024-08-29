#include <base/container/Collection.h>
#include <bsp-interface/di/led.h>
#include <DigitalLed.h>

/// @brief 数字 LED 灯集合。
/// @return
base::ICollection<std::string, bsp::IDigitalLed *> const &DI_DigitalLedCollection()
{
    static base::Collection<std::string, bsp::IDigitalLed *> collection{
        std::pair<std::string, bsp::IDigitalLed *>{"red_led", &bsp::RedDigitalLed::Instance()},
        std::pair<std::string, bsp::IDigitalLed *>{"green_led", &bsp::GreenDigitalLed::Instance()},
    };

    return collection;
}

bsp::IDigitalLed &DI_RedDigitalLed()
{
    return bsp::RedDigitalLed::Instance();
}

bsp::IDigitalLed &DI_GreenDigitalLed()
{
    return bsp::GreenDigitalLed::Instance();
}
