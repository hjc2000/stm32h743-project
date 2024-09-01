#pragma once
#include <base/SingletonGetter.h>
#include <bsp-interface/di/dma.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/serial/ISerial.h>
#include <hal-wrapper/peripheral/serial/SerialOptions.h>
#include <task/BinarySemaphore.h>
#include <task/Critical.h>
#include <task/Mutex.h>

namespace hal
{
    class Serial :
        public bsp::ISerial
    {
    private:
        Serial() = default;

        bool _have_begun = false;
        UART_HandleTypeDef _uart_handle{};
        task::BinarySemaphore _sending_completion_signal;
        task::BinarySemaphore _receiving_completion_signal;
        task::Mutex _read_lock{};

        bsp::IDmaChannel *_rx_dma_channel = nullptr;
        bsp::IDmaChannel *_tx_dma_channel = nullptr;

#pragma region 初始化
        void InitializeGpio();
        void InitializeDma();
        void HalUartInit(SerialOptions const &options);
        void InitializeInterrupt();
#pragma endregion

        /// @brief 通过串口句柄和 DMA 寄存器，获取当前 DMA 接收了多少个字节。
        /// @return
        int32_t HaveRead();

#pragma region 被中断处理函数回调的函数
        static void OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos);
        static void OnSendCompleteCallback(UART_HandleTypeDef *huart);
        static void OnReadTimeout(UART_HandleTypeDef *huart);
#pragma endregion

        void SetReadTimeoutByBaudCount(uint32_t value);

    public:
        static Serial &Instance()
        {
            class Getter : public base::SingletonGetter<Serial>
            {
            public:
                std::unique_ptr<Serial> Create() override
                {
                    return std::unique_ptr<Serial>{new Serial{}};
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

            Getter g;
            return g.Instance();
        }

        std::string Name() override
        {
            return "serial";
        }

#pragma region Stream
        /// @brief 调用后临时启动 DMA 接收一次数据。
        /// @note 本类没有缓冲机制，所以上层应用如果调用 Read 不及时，会丢失数据。
        /// @note 因为调用一次 Read 具有一定开销，需要设置寄存器，使能中断，设置一些
        /// 状态变量。所以为了提高效率，每次调用 Read 时传入的 buffer 适当大一些，
        /// 并且 count 大一些。
        ///
        /// @param buffer
        /// @param offset
        /// @param count
        /// @return
        int32_t Read(uint8_t *buffer, int32_t offset, int32_t count) override;

        /// @brief 调用后临时启动 DMA 进行一次发送。
        /// @param buffer
        /// @param offset
        /// @param count
        void Write(uint8_t const *buffer, int32_t offset, int32_t count) override;

        void Close() override;
#pragma endregion

        /// @brief 打开串口
        /// @param options
        void Open(bsp::ISerialOptions const &options) override;
    };
} // namespace hal
