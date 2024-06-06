#include"Serial.h"
#include<FreeRTOS.h>
#include<hal-wrapper/clock/SysTickClock.h>
#include<hal-wrapper/interrupt/Interrupt.h>
#include<hal-wrapper/peripheral/dma/DmaConfig.h>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>
#include<task.h>

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
		HAL_DMA_IRQHandler(&Serial::Instance()._tx_dma_handle);
	}

	void DMA_STR1_IRQHandler()
	{
		HAL_DMA_IRQHandler(&Serial::Instance()._rx_dma_handle);
	}
}
#pragma endregion

int32_t hal::Serial::HaveRead()
{
	return _uart_handle.RxXferSize - __HAL_DMA_GET_COUNTER(&_rx_dma_handle);
}

void Serial::OnMspInitCallback(UART_HandleTypeDef *huart)
{
	auto init_gpio = []()
	{
		hal::GpioPortA::Instance().EnableClock();
		__HAL_RCC_USART1_CLK_ENABLE();

		// 发送引脚 PA9
		hal::GpioPinConfig options;
		options._mode = hal::GpioPinMode::AlternateFunction_PushPull;
		options._pull_mode = hal::GpioPinPull::NoPull;
		options._speed = hal::GpioPinSpeed::High;
		options._alternate = hal::PA9Alternate::usart1;
		hal::GpioPortA::Instance().InitPin(hal::GpioPin::Pin9, options);

		// 接收引脚 PA10
		options._alternate = hal::PA10Alternate::usart1;
		hal::GpioPortA::Instance().InitPin(hal::GpioPin::Pin10, options);
	};

	auto init_tx_dma = []()
	{
		__HAL_RCC_DMA1_CLK_ENABLE();

		hal::DmaConfig dma_config { };
		dma_config._request = hal::DmaConfig::Request::Usart1Tx;
		dma_config._direction = hal::DmaConfig::Direction::MemoryToPeripheral;
		dma_config._peripheral_address_increase = hal::DmaConfig::PeripheralAddressIncrease::Disable;
		dma_config._memory_address_increase = hal::DmaConfig::MemoryAddressIncrease::Enable;
		dma_config._peripheral_data_alignment = hal::DmaConfig::PeripheralDataAlignment::Byte;
		dma_config._memory_data_alignment = hal::DmaConfig::MemoryDataAlignment::Byte;
		dma_config._mode = hal::DmaConfig::Mode::Normal;
		dma_config._priority = hal::DmaConfig::Priority::Medium;
		dma_config._fifo_mode = hal::DmaConfig::FifoMode::Disable;
		dma_config._fifo_threshold = hal::DmaConfig::FifoThreshold::Threshold_4_div_4;
		dma_config._memory_burst = hal::DmaConfig::MemoryBurst::Single;
		dma_config._peripheral_burst = hal::DmaConfig::PeripheralBurst::Single;

		Serial::Instance()._tx_dma_handle.Instance = DMA1_Stream0;
		Serial::Instance()._tx_dma_handle.Init = dma_config;
		HAL_DMA_Init(&Serial::Instance()._tx_dma_handle);
	};

	auto init_rx_dma = []()
	{
		__HAL_RCC_DMA1_CLK_ENABLE();

		hal::DmaConfig dma_config { };
		dma_config._request = hal::DmaConfig::Request::Usart1Rx;
		dma_config._direction = hal::DmaConfig::Direction::PeripheralToMemory;
		dma_config._peripheral_address_increase = hal::DmaConfig::PeripheralAddressIncrease::Disable;
		dma_config._memory_address_increase = hal::DmaConfig::MemoryAddressIncrease::Enable;
		dma_config._peripheral_data_alignment = hal::DmaConfig::PeripheralDataAlignment::Byte;
		dma_config._memory_data_alignment = hal::DmaConfig::MemoryDataAlignment::Byte;
		dma_config._mode = hal::DmaConfig::Mode::Normal;
		dma_config._priority = hal::DmaConfig::Priority::Medium;
		dma_config._fifo_mode = hal::DmaConfig::FifoMode::Disable;
		dma_config._fifo_threshold = hal::DmaConfig::FifoThreshold::Threshold_4_div_4;
		dma_config._memory_burst = hal::DmaConfig::MemoryBurst::Single;
		dma_config._peripheral_burst = hal::DmaConfig::PeripheralBurst::Single;

		Serial::Instance()._rx_dma_handle.Instance = DMA1_Stream1;
		Serial::Instance()._rx_dma_handle.Init = dma_config;
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
			// HAL_UART_Receive_DMA
			// HAL_UARTEx_ReceiveToIdle_DMA
			HAL_UART_Receive_DMA(&_uart_handle, buffer + offset, count);

			/*
			* 通过赋值为空指针，把传输半满回调给禁用，不然接收的数据较长，超过缓冲区一半时，
			* 即使是一次性接收的，UART 也会回调 OnReceiveEventCallback 两次。
			*
			* 这个操作需要在临界区中，并且 DMA 的中断要处于 freertos 的管理范围内，否则无效。
			*/
			_rx_dma_handle.XferHalfCpltCallback = nullptr;
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
		throw std::runtime_error { "发送失败" };
	}
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
	hal::Interrupt::DisableIRQ(IRQn_Type::DMA1_Stream1_IRQn);
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

#pragma region 属性
uint32_t Serial::BaudRate() const
{
	return _baud_rate;
}

void Serial::SetBaudRate(uint32_t value)
{
	if (_have_begun)
	{
		throw std::runtime_error { "串口打开后不允许更改设置" };
	}

	_baud_rate = value;
}

uint8_t Serial::DataBits() const
{
	return _data_bits;
}

void Serial::SetDataBits(uint8_t value)
{
	if (_have_begun)
	{
		throw std::runtime_error { "串口打开后不允许更改设置" };
	}

	_data_bits = value;
}

ISerial::ParityOption Serial::Parity() const
{
	return _parity;
}

void Serial::SetParity(ISerial::ParityOption value)
{
	if (_have_begun)
	{
		throw std::runtime_error { "串口打开后不允许更改设置" };
	}

	_parity = value;
}

ISerial::StopBitsOption Serial::StopBits() const
{
	return _stop_bits;
}

void Serial::SetStopBits(ISerial::StopBitsOption value)
{
	if (_have_begun)
	{
		throw std::runtime_error { "串口打开后不允许更改设置" };
	}

	_stop_bits = value;
}

ISerial::HardwareFlowControlOption Serial::HardwareFlowControl() const
{
	return _hardware_flow_control;
}

void Serial::SetHardwareFlowControl(ISerial::HardwareFlowControlOption value)
{
	if (_have_begun)
	{
		throw std::runtime_error { "串口打开后不允许更改设置" };
	}

	_hardware_flow_control = value;
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

void hal::Serial::SetReadTimeoutByFrameCount(uint32_t value)
{
	// 1 位起始位
	uint32_t baud_count = 1 * value;
	baud_count += DataBits() * value;
	if (Parity() != bsp::ISerial::ParityOption::None)
	{
		baud_count += 1 * value;
	}

	switch (StopBits())
	{
	case ISerial::StopBitsOption::ZeroPointFive:
		{
			baud_count += 1 * (value / 2);
			if (value % 2 > 0)
			{
				baud_count += 1;
			}

			break;
		}
	case ISerial::StopBitsOption::One:
		{
			baud_count += 1 * value;
			break;
		}
	case ISerial::StopBitsOption::OnePointFive:
		{
			baud_count += 3 * (value / 2);
			if (value % 2 > 0)
			{
				baud_count += 2;
			}

			break;
		}
	case ISerial::StopBitsOption::Tow:
		{
			baud_count += 2 * value;
			break;
		}
	default:
		{
			baud_count += 1 * value;
			break;
		}
	}

	SetReadTimeoutByBaudCount(baud_count);
}

void Serial::Open()
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

	hal::UartConfig config;
	config.Deserialize(*this);
	config._baud_rate = _baud_rate;

	_uart_handle.Instance = USART1;
	_uart_handle.Init = config;
	_uart_handle.MspInitCallback = OnMspInitCallback;

	HAL_UART_Init(&_uart_handle);
	SetReadTimeoutByFrameCount(2);

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
		hal::Interrupt::SetPriority(IRQn_Type::USART1_IRQn, 10, 0);
		hal::Interrupt::EnableIRQ(IRQn_Type::USART1_IRQn);

		hal::Interrupt::SetPriority(IRQn_Type::DMA1_Stream0_IRQn, 10, 0);
		hal::Interrupt::EnableIRQ(IRQn_Type::DMA1_Stream0_IRQn);

		hal::Interrupt::SetPriority(IRQn_Type::DMA1_Stream1_IRQn, 10, 0);
		hal::Interrupt::EnableIRQ(IRQn_Type::DMA1_Stream1_IRQn);
	};

	enable_interrupt();
}
