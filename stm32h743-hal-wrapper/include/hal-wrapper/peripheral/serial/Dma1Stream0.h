#pragma once
#include <bsp-interface/dma/IDmaChannel.h>
#include <hal.h>

namespace bsp
{
    class Dma1Stream0 :
        public bsp::IDmaChannel
    {
    private:
        DMA_HandleTypeDef _handle{};
        bool _is_open = false;

    public:
        static Dma1Stream0 &Instance()
        {
            static Dma1Stream0 o;
            return o;
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
