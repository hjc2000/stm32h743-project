#include "DmaStream.h"
#include <base/string/define.h>
#include <stdexcept>

void bsp::DmaStream::LinkDmaToUartTx(UART_HandleTypeDef &uart)
{
    uart.hdmatx = &_dma_handle;
    _dma_handle.Parent = &uart;
}

void bsp::DmaStream::LinkDmaToUartRx(UART_HandleTypeDef &uart)
{
    uart.hdmarx = &_dma_handle;
    _dma_handle.Parent = &uart;
}

void bsp::DmaStream::InitializeDmaProperty(
    bsp::dma::property::PeripheralIncrement const &peripheral_increment,
    bsp::dma::property::MemoryIncrement const &memory_increment,
    bsp::dma::property::PeripheralDataAlignment const &peripheral_data_alignment,
    bsp::dma::property::MemoryDataAlignment const &memory_data_alignment,
    bsp::dma::property::Priority priority,
    std::string const &request)
{
    _dma_handle.Init.Mode = DMA_NORMAL;
    _dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    _dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    _dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    _dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    _dma_handle.Init.PeriphInc = peripheral_increment.Value() ? DMA_PINC_ENABLE : DMA_PINC_DISABLE;
    _dma_handle.Init.MemInc = memory_increment.Value() ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;

    switch (peripheral_data_alignment.Value())
    {
    case 1:
        {
            _dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            break;
        }
    case 2:
        {
            _dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
            break;
        }
    case 4:
        {
            _dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
            break;
        }
    default:
        {
            throw std::invalid_argument{CODE_POS_STR + "非法对齐方式"};
        }
    }

    switch (memory_data_alignment.Value())
    {
    case 1:
        {
            _dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            break;
        }
    case 2:
        {
            _dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
            break;
        }
    case 4:
        {
            _dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
            break;
        }
    default:
        {
            throw std::invalid_argument{CODE_POS_STR + "非法对齐方式"};
        }
    }

    switch (priority)
    {
    case bsp::dma::property::Priority::Low:
        {
            _dma_handle.Init.Priority = DMA_PRIORITY_LOW;
            break;
        }
    case bsp::dma::property::Priority::Medium:
        {
            _dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
            break;
        }
    case bsp::dma::property::Priority::High:
        {
            _dma_handle.Init.Priority = DMA_PRIORITY_HIGH;
            break;
        }
    case bsp::dma::property::Priority::VeryHigh:
        {
            _dma_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
            break;
        }
    default:
        {
            throw std::invalid_argument{CODE_POS_STR + "非法优先级"};
        }
    }

    // Request
    {
        auto it = _request_map.find(request);
        if (it == _request_map.end())
        {
            throw std::runtime_error{CODE_POS_STR + "不支持此请求"};
        }

        _dma_handle.Init.Request = it->second;
    }
}

void bsp::DmaStream::LinkDmaToParent(void *parent)
{
    switch (_dma_handle.Init.Request)
    {
    case DMA_REQUEST_USART1_TX:
        {
            LinkDmaToUartTx(*static_cast<UART_HandleTypeDef *>(parent));
            break;
        }
    case DMA_REQUEST_USART1_RX:
        {
            LinkDmaToUartRx(*static_cast<UART_HandleTypeDef *>(parent));
            break;
        }
    default:
        {
            throw std::runtime_error{CODE_POS_STR + "不支持的请求"};
        }
    }
}
