#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/expanded_io/PCF8574.h>

namespace
{
    class Initializer
    {
    private:
        bsp::PCF8574 _ex_io{
            "ex_io",
            DI_GpioPinCollection().Get("PB12"),
            DI_IicHostCollection().Get("gpio_iic_host"),
            0,
        };

        void Add(bsp::IExpandedIoPort &port)
        {
            _dic.Add(port.Name(), &port);
        }

    public:
        Initializer()
        {
            Add(_ex_io);
        }

        base::Dictionary<std::string, bsp::IExpandedIoPort *> _dic;
    };

    class Getter :
        public base::SingletonGetter<Initializer>
    {
    public:
        std::unique_ptr<Initializer> Create() override
        {
            return std::unique_ptr<Initializer>{new Initializer{}};
        }

        void Lock() override
        {
            DI_DisableGlobalInterrupt();
        }

        void Unlock() override
        {
            DI_EnableGlobalInterrupt();
        }
    };
} // namespace

base::IDictionary<std::string, bsp::IExpandedIoPort *> const &DI_ExpandedIoPortCollection()
{
    Getter o;
    return o.Instance()._dic;
}
