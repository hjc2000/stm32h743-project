#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/serial/GpioSoftwareIicHost.h>

namespace
{
    class Initializer
    {
    private:
        /// @brief 连接着 EEROM 芯片的软件模拟 IIC 主机接口。
        bsp::GpioSoftwareIicHost _eerom_iic_host{"eerom_iic_host", "PH4", "PH5"};

        void Add(bsp::IIicHost *host)
        {
            _dic.Add(host->Name(), host);
        }

    public:
        Initializer()
        {
            Add(&_eerom_iic_host);
        }

        base::Dictionary<std::string, bsp::IIicHost *> _dic;
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
            DI_InterruptSwitch().DisableGlobalInterrupt();
        }

        void Unlock() override
        {
            DI_InterruptSwitch().EnableGlobalInterrupt();
        }
    };
} // namespace

base::IDictionary<std::string, bsp::IIicHost *> const &DI_IicHostCollection()
{
    Getter o;
    return o.Instance()._dic;
}
