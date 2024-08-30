#include "LinkDma.h"

void bsp::LinkDmaToParent(DMA_HandleTypeDef &dma, void *parent)
{
    switch (dma.Init.Request)
    {
    case DMA_REQUEST_USART1_TX:
        {
            bsp::LinkDmaToUartTx(dma, *static_cast<UART_HandleTypeDef *>(parent));
            break;
        }
    case DMA_REQUEST_USART1_RX:
        {
            bsp::LinkDmaToUartRx(dma, *static_cast<UART_HandleTypeDef *>(parent));
            break;
        }
    }
}

void bsp::LinkDmaToUartTx(DMA_HandleTypeDef &dma, UART_HandleTypeDef &uart)
{
    uart.hdmatx = &dma;
    dma.Parent = &uart;
}

void bsp::LinkDmaToUartRx(DMA_HandleTypeDef &dma, UART_HandleTypeDef &uart)
{
    uart.hdmarx = &dma;
    dma.Parent = &uart;
}
