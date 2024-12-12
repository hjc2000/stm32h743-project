#include "EhternetPort.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <EthernetController.h>
#include <hal.h>

bsp::EhternetPort &bsp::EhternetPort::Instance()
{
    class Getter :
        public base::SingletonGetter<EhternetPort>
    {
    public:
        std::unique_ptr<EhternetPort> Create() override
        {
            return std::unique_ptr<EhternetPort>{new EhternetPort{}};
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

    Getter g;
    return g.Instance();
}

std::string bsp::EhternetPort::Name() const
{
    return "eth";
}

void bsp::EhternetPort::Open(base::Mac const &mac)
{
    ResetPHY();
    DI_EthernetController().Open(bsp::IEthernetController_InterfaceType::RMII,
                                 0,
                                 mac);
}

uint32_t bsp::EhternetPort::ReadPHYRegister(uint32_t register_index)
{
    return _controller->ReadPHYRegister(register_index);
}

void bsp::EhternetPort::WritePHYRegister(uint32_t register_index, uint32_t value)
{
    _controller->WritePHYRegister(register_index, value);
}

void bsp::EhternetPort::ResetPHY()
{
    DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0); /* 硬件复位 */
    DI_Delayer().Delay(std::chrono::milliseconds{100});
    DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1); /* 复位结束 */
    DI_Delayer().Delay(std::chrono::milliseconds{100});
}
