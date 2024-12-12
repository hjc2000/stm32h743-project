#include "EhternetPort.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <EthernetController.h>
#include <hal.h>

bsp::EhternetPort::EhternetPort()
{
}

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
    return std::string();
}

uint32_t bsp::EhternetPort::ReadPHYRegister(uint32_t register_index)
{
    return 0;
}

void bsp::EhternetPort::WritePHYRegister(uint32_t register_index, uint32_t value)
{
}

void bsp::EhternetPort::ResetPHY()
{
    if (_controller == nullptr)
    {
        throw std::runtime_error{"本 EhternetPort 对象持有的以太网控制器是个空指针，"
                                 "请先调用 Open 方法，传入一个以太网控制器"};
    }
}
