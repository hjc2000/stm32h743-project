#pragma once
#include <bsp-interface/di/interrupt.h>
#include <DmaStream.h>

namespace bsp
{
    class Dma1Stream1 :
        public bsp::DmaStream
    {
    private:
        Dma1Stream1() = default;

        bool _is_open = false;

    public:
        static_function Dma1Stream1 &Instance();

        std::string Name() const override;

        /// @brief 打开 DMA，模式为将数据从外设拷贝到内存。
        /// @param parent DMA 要被连接到的父设备的句柄。例如对于 HAL 库的 USART，就是 UART_HandleTypeDef。
        /// @param peripheral_increment
        /// @param memory_increment
        /// @param peripheral_data_alignment
        /// @param memory_data_alignment
        /// @param priority
        /// @param request
        virtual void OpenAsPeripheralToMemoryMode(void *parent,
                                                  bsp::dma::property::PeripheralIncrement const &peripheral_increment,
                                                  bsp::dma::property::MemoryIncrement const &memory_increment,
                                                  bsp::dma::property::PeripheralDataAlignment const &peripheral_data_alignment,
                                                  bsp::dma::property::MemoryDataAlignment const &memory_data_alignment,
                                                  bsp::dma::property::Priority priority,
                                                  std::string const &request);

        /// @brief 打开 DMA，模式为将数据从内存拷贝到外设。
        /// @param parent DMA 要被连接到的父设备的句柄。例如对于 HAL 库的 USART，就是 UART_HandleTypeDef。
        /// @param peripheral_increment
        /// @param memory_increment
        /// @param peripheral_data_alignment
        /// @param memory_data_alignment
        /// @param priority
        /// @param request
        virtual void OpenAsMemoryToPeripheralMode(void *parent,
                                                  bsp::dma::property::PeripheralIncrement const &peripheral_increment,
                                                  bsp::dma::property::MemoryIncrement const &memory_increment,
                                                  bsp::dma::property::PeripheralDataAlignment const &peripheral_data_alignment,
                                                  bsp::dma::property::MemoryDataAlignment const &memory_data_alignment,
                                                  bsp::dma::property::Priority priority,
                                                  std::string const &request);

        /// @brief 打开 DMA，模式为将数据从内存拷贝到内存。
        /// @param parent DMA 要被连接到的父设备的句柄。例如对于 HAL 库的 USART，就是 UART_HandleTypeDef。
        /// @param peripheral_increment
        /// @param memory_increment
        /// @param peripheral_data_alignment
        /// @param memory_data_alignment
        /// @param priority
        /// @param request
        virtual void OpenAsMomoryToMemoryMode(void *parent,
                                              bsp::dma::property::PeripheralIncrement const &peripheral_increment,
                                              bsp::dma::property::MemoryIncrement const &memory_increment,
                                              bsp::dma::property::PeripheralDataAlignment const &peripheral_data_alignment,
                                              bsp::dma::property::MemoryDataAlignment const &memory_data_alignment,
                                              bsp::dma::property::Priority priority,
                                              std::string const &request);

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
