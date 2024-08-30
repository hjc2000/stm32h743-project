#pragma once
#include <hal.h>

namespace bsp
{
    void LinkDmaToParent(DMA_HandleTypeDef &dma, void *parent);
    void LinkDmaToUartTx(DMA_HandleTypeDef &dma, UART_HandleTypeDef &uart);
    void LinkDmaToUartRx(DMA_HandleTypeDef &dma, UART_HandleTypeDef &uart);
} // namespace bsp
