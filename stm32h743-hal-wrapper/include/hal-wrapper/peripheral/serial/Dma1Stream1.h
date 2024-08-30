#pragma once
#include <base/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/dma/IDmaChannel.h>
#include <hal.h>

namespace bsp
{
    class Dma1Stream1 :
        public bsp::IDmaChannel
    {
    private:
        DMA_HandleTypeDef _dma_handle{};
        bool _is_open = false;

    public:
        static Dma1Stream1 &Instance()
        {
            class Getter : public base::SingletonGetter<Dma1Stream1>
            {
            public:
                std::unique_ptr<Dma1Stream1> Create() override
                {
                    return std::unique_ptr<Dma1Stream1>{new Dma1Stream1{}};
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

        std::string Name() const override;
        void Open(bsp::IDmaOptions const &options, void *parent) override;

        bool IsOpen() const override
        {
            return _is_open;
        }

        void Close() override
        {
            _is_open = false;
        }
    };
} // namespace bsp
