#include"Serial.h"
#include<bsp/bsp.h>
#include<FreeRTOS.h>
#include<hal-wrapper/interrupt/Interrupt.h>
#include<hal-wrapper/peripheral/dma/DmaConfig.h>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>
#include<task.h>

using namespace bsp;

#pragma region 中断处理函数
extern "C"
{
	void USART1_IRQHandler()
	{
		HAL_UART_IRQHandler(&Serial::Instance()._uart_handle);
	}

	void DMA1_Channel4_IRQHandler()
	{
		HAL_DMA_IRQHandler(&Serial::Instance()._tx_dma_handle);
	}

	void DMA1_Channel5_IRQHandler()
	{
		HAL_DMA_IRQHandler(&Serial::Instance()._rx_dma_handle);
	}
}
#pragma endregion

void Serial::OnMspInitCallback(UART_HandleTypeDef *huart)
{
	auto init_gpio = []()
	{
		hal::GpioPortA::Instance().EnableClock();
		__HAL_RCC_USART1_CLK_ENABLE();

		// 发送引脚 PA9
		hal::GpioPinConfig options;
		options._mode = hal::GpioPinMode::AlternateFunction_PushPull;
		options._pull_mode = hal::GpioPinPull::PullUp;
		options._speed = hal::GpioPinSpeed::High;
		hal::GpioPortA::Instance().InitPin(hal::GpioPin::Pin9, options);

		// 接收引脚 PA10
		options._mode = hal::GpioPinMode::AlternateFunction_PushPull;
		hal::GpioPortA::Instance().InitPin(hal::GpioPin::Pin10, options);
	};

	auto init_tx_dma = []()
	{
		__HAL_RCC_DMA1_CLK_ENABLE();
		hal::DmaConfig options { };
		HAL_DMA_Init(&Serial::Instance()._tx_dma_handle);
	};

	auto init_rx_dma = []()
	{
		__HAL_RCC_DMA1_CLK_ENABLE();
		hal::DmaConfig options { };
		HAL_DMA_Init(&Serial::Instance()._rx_dma_handle);
	};

	// 连接到 DMA 发送通道
	auto link_dma_channel = []()
	{
		Serial::Instance()._uart_handle.hdmatx = &Serial::Instance()._tx_dma_handle;
		Serial::Instance()._tx_dma_handle.Parent = &Serial::Instance()._uart_handle;

		Serial::Instance()._uart_handle.hdmarx = &Serial::Instance()._rx_dma_handle;
		Serial::Instance()._rx_dma_handle.Parent = &Serial::Instance()._uart_handle;
	};

	init_gpio();
	init_tx_dma();
	init_rx_dma();
	link_dma_channel();
}

#pragma region 被中断处理函数回调的函数
void bsp::Serial::OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos)
{
	Serial::Instance()._current_receive_count = pos;
	Serial::Instance()._receive_complete_signal.ReleaseFromISR();
}

void bsp::Serial::OnSendCompleteCallback(UART_HandleTypeDef *huart)
{
	Serial::Instance()._send_complete_signal.ReleaseFromISR();
}
#pragma endregion

#pragma region Stream
bool Serial::CanRead()
{
	return true;
}

bool Serial::CanWrite()
{
	return true;
}

bool Serial::CanSeek()
{
	return false;
}

int64_t Serial::Length()
{
	return 0;
}

void Serial::SetLength(int64_t value)
{
	// 不支持的操作
}

int32_t Serial::Read(uint8_t *buffer, int32_t offset, int32_t count)
{
	if (count > UINT16_MAX)
	{
		throw std::invalid_argument { "count 太大" };
	}

	task::MutexLockGuard l { _read_lock };
	while (true)
	{
		task::Critical::Run([&]()
		{
			HAL_UARTEx_ReceiveToIdle_DMA(&_uart_handle, buffer + offset, count);

			/*
			* 通过赋值为空指针，把传输半满回调给禁用，不然接收的数据较长，超过缓冲区一半时，
			* 即使是一次性接收的，UART 也会回调 OnReceiveEventCallback 两次。
			*
			* 这个操作需要在临界区中，并且 DMA 的中断要处于 freertos 的管理范围内，否则无效。
			*/
			_rx_dma_handle.XferHalfCpltCallback = nullptr;
		});

		_receive_complete_signal.Acquire();
		if (_current_receive_count > 0)
		{
			return _current_receive_count;
		}
	}
}

void Serial::Write(uint8_t const *buffer, int32_t offset, int32_t count)
{
	_send_complete_signal.Acquire();
	HAL_UART_Transmit_DMA(&_uart_handle, buffer + offset, count);
}

void Serial::Flush()
{
	// Write 方法利用 DMA 直接发送缓冲区，本类没有内部缓冲区，不需要冲洗。
}

void Serial::Close()
{
	HAL_UART_DMAStop(&_uart_handle);
	hal::Interrupt::DisableIRQ(IRQn_Type::USART1_IRQn);
	hal::Interrupt::DisableIRQ(IRQn_Type::DMA1_Stream0_IRQn);
	hal::Interrupt::DisableIRQ(IRQn_Type::DMA1_Stream0_IRQn);
	_have_begun = false;
}

int64_t Serial::Position()
{
	return 0;
}

void Serial::SetPosition(int64_t value)
{
	// 不支持的操作。
}
#pragma endregion

void Serial::Begin(uint32_t baud_rate)
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

	_baud_rate = baud_rate;
	hal::UartConfig options;
	options._baud_rate = baud_rate;

	_uart_handle.Instance = _uart_hardware_instance;
	_uart_handle.Init = options;
	_uart_handle.MspInitCallback = OnMspInitCallback;
	HAL_UART_Init(&_uart_handle);

	/*
	* HAL_UART_Init 函数会把中断处理函数中回调的函数都设为默认的，所以必须在 HAL_UART_Init
	* 之后对函数指针赋值。
	*/
	_uart_handle.RxEventCallback = OnReceiveEventCallback;
	_uart_handle.TxCpltCallback = OnSendCompleteCallback;

	// 启用中断
	auto enable_interrupt = []()
	{
		hal::Interrupt::SetPriority(IRQn_Type::USART1_IRQn, 10, 0);
		hal::Interrupt::EnableIRQ(IRQn_Type::USART1_IRQn);

		hal::Interrupt::SetPriority(IRQn_Type::DMA1_Stream0_IRQn, 10, 0);
		hal::Interrupt::EnableIRQ(IRQn_Type::DMA1_Stream0_IRQn);

		hal::Interrupt::SetPriority(IRQn_Type::DMA1_Stream1_IRQn, 10, 0);
		hal::Interrupt::EnableIRQ(IRQn_Type::DMA1_Stream1_IRQn);
	};

	enable_interrupt();
}
