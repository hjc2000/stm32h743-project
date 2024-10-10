#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/led.h>
#include <DigitalLed.h>

/// @brief 数字 LED 灯集合。
/// @return
base::IDictionary<std::string, bsp::IDigitalLed *> const &DI_DigitalLedCollection()
{
    class Getter :
        public base::SingletonGetter<base::Dictionary<std::string, bsp::IDigitalLed *>>
    {
    protected:
        std::unique_ptr<base::Dictionary<std::string, bsp::IDigitalLed *>> Create() override
        {
            return std::unique_ptr<base::Dictionary<std::string, bsp::IDigitalLed *>>{
                new base::Dictionary<std::string, bsp::IDigitalLed *>{
                    std::pair<std::string, bsp::IDigitalLed *>{"red_led", &bsp::RedDigitalLed::Instance()},
                    std::pair<std::string, bsp::IDigitalLed *>{"green_led", &bsp::GreenDigitalLed::Instance()},
                },
            };
        }

        void Lock() override
        {
            DI_InterruptSwitch().DisableGlobalInterrupt();
        }

        void Unlock() override
        {
            DI_InterruptSwitch().EnableGlobalInterrupt();
        }
    };

    Getter o;
    return o.Instance();
}

bsp::IDigitalLed &DI_RedDigitalLed()
{
    return bsp::RedDigitalLed::Instance();
}

bsp::IDigitalLed &DI_GreenDigitalLed()
{
    return bsp::GreenDigitalLed::Instance();
}
