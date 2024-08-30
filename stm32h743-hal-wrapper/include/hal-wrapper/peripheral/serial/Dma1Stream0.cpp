#include "Dma1Stream0.h"
#include "DmaOptions.h"
#include "LinkDma.h"

std::string bsp::Dma1Stream0::Name() const
{
    return "dma1_stream0";
}

void bsp::Dma1Stream0::Open(bsp::IDmaOptions const &options, void *parent)
{
    if (_is_open)
    {
        throw std::runtime_error{"已经打开，要先关闭才能再次打开"};
    }

    _is_open = true;

    __HAL_RCC_DMA1_CLK_ENABLE();
    _dma_handle.Instance = DMA1_Stream0;
    _dma_handle.Init = static_cast<bsp::DmaOptions const &>(options);
    HAL_DMA_Init(&_dma_handle);
    bsp::LinkDmaToParent(_dma_handle, parent);
}
