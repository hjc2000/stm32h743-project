#include "LinkDma.h"

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
