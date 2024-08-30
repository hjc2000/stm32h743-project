#include "Dma1Stream0.h"
#include "DmaOptions.h"

std::string bsp::Dma1Stream0::Name() const
{
    return "dma1_channel5";
}

void bsp::Dma1Stream0::Open(bsp::IDmaOptions const &options)
{
    if (_is_open)
    {
        throw std::runtime_error{"已经打开，要先关闭才能再次打开"};
    }

    _is_open = true;

    __HAL_RCC_DMA1_CLK_ENABLE();
    _handle.Instance = DMA1_Stream0;
    _handle.Init = static_cast<bsp::DmaOptions const &>(options);
    HAL_DMA_Init(&_handle);
}
