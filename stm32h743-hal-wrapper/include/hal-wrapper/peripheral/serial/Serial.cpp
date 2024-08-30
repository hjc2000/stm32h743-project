#include "Serial.h"
#include "DmaOptions.h"
#include "LinkDma.h"
#include <bsp-interface/di/dma.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>
#include <FreeRTOS.h>
#include <hal-wrapper/clock/SysTickClock.h>
#include <hal-wrapper/peripheral/dma/DmaConfig.h>
#include <task.h>

using namespace bsp;
using namespace hal;

#pragma region 中断处理函数
extern "C"
{
    void USART1_IRQHandler()
    {
        HAL_UART_IRQHandler(&Serial::Instance()._uart_handle);
    }

    void DMA_STR0_IRQHandler()
    {
        HAL_DMA_IRQHandler(Serial::Instance()._uart_handle.hdmatx);
    }

    void DMA_STR1_IRQHandler()
    {
        HAL_DMA_IRQHandler(Serial::Instance()._uart_handle.hdmarx);
    }
}
#pragma endregion

int32_t hal::Serial::HaveRead()
{
    return _uart_handle.RxXferSize - __HAL_DMA_GET_COUNTER(_uart_handle.hdmarx);
}

void Serial::OnMspInitCallback(UART_HandleTypeDef *huart)
{
    // 初始化 GPIO
    {
        __HAL_RCC_USART1_CLK_ENABLE();

        {
            // 发送引脚 PA9
            auto options = DICreate_GpioPinOptions();
            options->SetAlternateFunction("usart1");
            options->SetDriver(bsp::IGpioPinDriver::PushPull);
            options->SetPullMode(bsp::IGpioPinPullMode::NoPull);
            options->SetSpeedLevel(3);
            options->SetWorkMode(bsp::IGpioPinWorkMode::AlternateFunction);

            auto pin = DI_GpioPinCollection().Get("PA9");
            pin->Open(*options);
        }

        {
            // 接收引脚 PA10
            auto options = DICreate_GpioPinOptions();
            options->SetAlternateFunction("usart1");
            options->SetDriver(bsp::IGpioPinDriver::PushPull);
            options->SetPullMode(bsp::IGpioPinPullMode::NoPull);
            options->SetSpeedLevel(3);
            options->SetWorkMode(bsp::IGpioPinWorkMode::AlternateFunction);

            auto pin = DI_GpioPinCollection().Get("PA10");
            pin->Open(*options);
        }
    }

    // 初始化发送 DMA
    {
        auto options = DICreate_DmaOptions();
        options->SetDirection(bsp::IDmaOptions_Direction::MemoryToPeripheral);
        options->SetMemoryDataAlignment(1);
        options->SetMemoryIncrement(true);
        options->SetPeripheralDataAlignment(1);
        options->SetPeripheralIncrement(false);
        options->SetPriority(bsp::IDmaOptions_Priority::Medium);
        options->SetRequest("usart1_tx");
        DI_DmaChannelCollection().Get("dma1_stream0")->Open(*options, &Serial::Instance()._uart_handle);
    }

    // 初始化接受 DMA
    {
        auto options = DICreate_DmaOptions();
        options->SetDirection(bsp::IDmaOptions_Direction::PeripheralToMemory);
        options->SetMemoryDataAlignment(1);
        options->SetMemoryIncrement(true);
        options->SetPeripheralDataAlignment(1);
        options->SetPeripheralIncrement(false);
        options->SetPriority(bsp::IDmaOptions_Priority::Medium);
        options->SetRequest("usart1_rx");
        DI_DmaChannelCollection().Get("dma1_stream1")->Open(*options, &Serial::Instance()._uart_handle);
    }
}

#pragma region 被中断处理函数回调的函数

void Serial::OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos)
{
    Serial::Instance()._receive_complete_signal.ReleaseFromISR();
}

void Serial::OnSendCompleteCallback(UART_HandleTypeDef *huart)
{
    Serial::Instance()._send_complete_signal.ReleaseFromISR();
}

void hal::Serial::OnReadTimeout(UART_HandleTypeDef *huart)
{
    Serial::Instance()._receive_complete_signal.ReleaseFromISR();
}

#pragma endregion

#pragma region Stream

int32_t Serial::Read(uint8_t *buffer, int32_t offset, int32_t count)
{
    if (count > UINT16_MAX)
    {
        throw std::invalid_argument{"count 太大"};
    }

    task::MutexLockGuard l{_read_lock};
    while (true)
    {
        task::Critical::Run(
            [&]()
            {
                // HAL_UART_Receive_DMA
                // HAL_UARTEx_ReceiveToIdle_DMA
                HAL_UART_Receive_DMA(&_uart_handle, buffer + offset, count);

                /*
                 * 通过赋值为空指针，把传输半满回调给禁用，不然接收的数据较长，超过缓冲区一半时，
                 * 即使是一次性接收的，UART 也会回调 OnReceiveEventCallback 两次。
                 *
                 * 这个操作需要在临界区中，并且 DMA 的中断要处于 freertos 的管理范围内，否则无效。
                 */
                Serial::Instance()._uart_handle.hdmarx->XferHalfCpltCallback = nullptr;
            });

        _receive_complete_signal.Acquire();
        if (HaveRead() > 0)
        {
            return HaveRead();
        }
    }
}

void Serial::Write(uint8_t const *buffer, int32_t offset, int32_t count)
{
    _send_complete_signal.Acquire();
    HAL_StatusTypeDef ret = HAL_UART_Transmit_DMA(&_uart_handle, buffer + offset, count);
    if (ret != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"发送失败"};
    }
}

void Serial::Close()
{
    HAL_UART_DMAStop(&_uart_handle);
    DI_InterruptSwitch().DisableInterrupt(static_cast<uint32_t>(IRQn_Type::USART1_IRQn));
    DI_InterruptSwitch().DisableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn));
    DI_InterruptSwitch().DisableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn));
    _have_begun = false;
}

#pragma endregion

UART_HandleTypeDef &hal::Serial::Handle()
{
    return _uart_handle;
}

void hal::Serial::SetReadTimeoutByBaudCount(uint32_t value)
{
    if (value > 0)
    {
        HAL_UART_ReceiverTimeout_Config(&Handle(), value);
        HAL_UART_EnableReceiverTimeout(&Handle());
    }
    else
    {
        HAL_UART_DisableReceiverTimeout(&Handle());
    }
}

void hal::Serial::Open(bsp::ISerialOptions const &options)
{
    if (_have_begun)
    {
        return;
    }

    _have_begun = true;

    /*
     * 先立刻释放一次信号量，等会 Write 方法被调用时直接通过，不被阻塞。
     * 然后在发送完成之前，第二次 Write 就会被阻塞了，这还能防止 Write
     * 被多线程同时调用。
     */
    _send_complete_signal.Release();

    _uart_handle.Instance = USART1;
    SerialOptions const &serial_options = static_cast<SerialOptions const &>(options);
    _uart_handle.Init = serial_options;
    _uart_handle.MspInitCallback = OnMspInitCallback;
    HAL_UART_Init(&_uart_handle);

    // 超时在串口初始化后设置才有效
    SetReadTimeoutByBaudCount(serial_options.CalculateFramesBaudCount(2));

    /*
     * HAL_UART_Init 函数会把中断处理函数中回调的函数都设为默认的，所以必须在 HAL_UART_Init
     * 之后对函数指针赋值。
     */
    _uart_handle.RxEventCallback = OnReceiveEventCallback;
    _uart_handle.TxCpltCallback = OnSendCompleteCallback;
    _uart_handle.ErrorCallback = OnReadTimeout;

    // 启用中断
    auto enable_interrupt = []()
    {
        DI_InterruptSwitch().EnableInterrupt(static_cast<uint32_t>(IRQn_Type::USART1_IRQn), 10);
        DI_InterruptSwitch().EnableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn), 10);
        DI_InterruptSwitch().EnableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn), 10);
    };

    enable_interrupt();
}
