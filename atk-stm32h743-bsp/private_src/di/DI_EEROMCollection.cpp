#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/eerom.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/eerom/AT24C02_EEROM.h>

namespace
{
    class Initializer
    {
    private:
        bsp::AT24C02_EEROM _at24c02{"at24c02", DI_IicHostCollection().Get("gpio_iic_host")};

        void Add(bsp::IEEROM *eerom)
        {
            _dic.Add(eerom->Name(), eerom);
        }

    public:
        Initializer()
        {
            Add(&_at24c02);
        }

        base::Dictionary<std::string, bsp::IEEROM *> _dic;
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

/// @brief EEROM 储存器集合。
/// @return
base::IDictionary<std::string, bsp::IEEROM *> const &DI_EEROMCollection()
{
    Getter o;
    return o.Instance()._dic;
}
