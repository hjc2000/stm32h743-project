#include "Serial.h"
#include "base/container/Dictionary.h"
#include "base/peripheral/ISerial.h"
#include "bsp-interface/di/dma.h"
#include "bsp-interface/di/gpio.h"
#include "bsp-interface/di/interrupt.h"
#include "bsp-interface/TaskSingletonGetter.h"
#include "FreeRTOS.h"
#include "task.h"

/* #region 初始化 */

void bsp::Serial::InitializeGpio()
{
	__HAL_RCC_USART1_CLK_ENABLE();

	// 发送引脚 PA9
	{
		auto pin = DI_GpioPinCollection().Get("PA9");

		pin->OpenAsAlternateFunctionMode("usart1",
										 bsp::IGpioPinPullMode::PullUp,
										 bsp::IGpioPinDriver::PushPull);
	}

	// 接收引脚 PA10
	{
		auto pin = DI_GpioPinCollection().Get("PA10");

		pin->OpenAsAlternateFunctionMode("usart1",
										 bsp::IGpioPinPullMode::PullUp,
										 bsp::IGpioPinDriver::PushPull);
	}
}

void bsp::Serial::InitializeDma()
{
	// 初始化发送 DMA
	{
		_tx_dma_channel = DI_DmaChannelCollection().Get("dma1_stream0");

		_tx_dma_channel->OpenAsMemoryToPeripheralMode(&_uart_handle,
													  bsp::dma::property::PeripheralIncrement{false},
													  bsp::dma::property::MemoryIncrement{true},
													  bsp::dma::property::PeripheralDataAlignment{1},
													  bsp::dma::property::MemoryDataAlignment{1},
													  bsp::dma::property::Priority::Medium,
													  "usart1_tx");
	}

	// 初始化接收 DMA
	{
		_rx_dma_channel = DI_DmaChannelCollection().Get("dma1_stream1");

		_rx_dma_channel->OpenAsPeripheralToMemoryMode(&_uart_handle,
													  bsp::dma::property::PeripheralIncrement{false},
													  bsp::dma::property::MemoryIncrement{true},
													  bsp::dma::property::PeripheralDataAlignment{1},
													  bsp::dma::property::MemoryDataAlignment{1},
													  bsp::dma::property::Priority::Medium,
													  "usart1_rx");
	}
}

void bsp::Serial::InitializeUart()
{
	_uart_handle.Instance = USART1;

	switch (_direction)
	{
	case base::serial::Direction::RX:
		{
			_uart_handle.Init.Mode = UART_MODE_RX;
			break;
		}
	case base::serial::Direction::TX:
		{
			_uart_handle.Init.Mode = UART_MODE_TX;
			break;
		}
	default:
	case base::serial::Direction::RX_TX:
		{
			_uart_handle.Init.Mode = UART_MODE_TX_RX;
			break;
		}
	}

	_uart_handle.Init.BaudRate = _baud_rate;

	switch (_data_bits)
	{
	default:
	case 8:
		{
			_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
			break;
		}
	case 9:
		{
			_uart_handle.Init.WordLength = UART_WORDLENGTH_9B;
			break;
		}
	}

	switch (_parity)
	{
	default:
	case base::serial::Parity::None:
		{
			_uart_handle.Init.Parity = UART_PARITY_NONE;
			break;
		}
	case base::serial::Parity::Even:
		{
			_uart_handle.Init.Parity = UART_PARITY_EVEN;
			break;
		}
	case base::serial::Parity::Odd:
		{
			_uart_handle.Init.Parity = UART_PARITY_ODD;
			break;
		}
	}

	switch (_stop_bits)
	{
	default:
	case base::serial::StopBits::One:
		{
			_uart_handle.Init.StopBits = UART_STOPBITS_1;
			break;
		}
	case base::serial::StopBits::Tow:
		{
			_uart_handle.Init.StopBits = UART_STOPBITS_2;
			break;
		}
	}

	switch (_hardware_flow_control)
	{
	default:
	case base::serial::HardwareFlowControl::None:
		{
			_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			break;
		}
	case base::serial::HardwareFlowControl::RTS:
		{
			_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_RTS;
			break;
		}
	case base::serial::HardwareFlowControl::CTS:
		{
			_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_CTS;
			break;
		}
	case base::serial::HardwareFlowControl::RTS_CTS:
		{
			_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
			break;
		}
	}

	// 16 倍过采样。每一个位被采样 16 次，最后可能是根据平均值来确定它的电平。
	_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;

	/**
	 * 每一次采样是使用 1 位的 ADC 采样还是使用 3 位的 ADC 采样。
	 * UART_ONE_BIT_SAMPLE_DISABLE 表示使用 3 位的 ADC 采样。
	 */
	_uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

	// 对 PCLK 的分频系数。
	_uart_handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	_uart_handle.MspInitCallback = nullptr;
	HAL_UART_Init(&_uart_handle);

	/*
	 * HAL_UART_Init 函数会把中断处理函数中回调的函数都设为默认的，所以必须在 HAL_UART_Init
	 * 之后对函数指针赋值。
	 */
	_uart_handle.RxEventCallback = OnReceiveEventCallback;
	_uart_handle.TxCpltCallback = OnSendCompleteCallback;
	_uart_handle.ErrorCallback = OnReadTimeout;

	// 超时在串口初始化后设置才有效
	SetReadTimeoutByBaudCount(FramesBaudCount(2));
}

void bsp::Serial::InitializeInterrupt()
{
	bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::USART1_IRQn),
											[this]()
											{
												HAL_UART_IRQHandler(&_uart_handle);
											});

	bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn),
											[this]()
											{
												HAL_DMA_IRQHandler(_uart_handle.hdmatx);
											});

	bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn),
											[this]()
											{
												HAL_DMA_IRQHandler(_uart_handle.hdmarx);
											});

	bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::USART1_IRQn), 10);
	bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn), 10);
	bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn), 10);
}

/* #endregion */

void bsp::Serial::SetReadTimeoutByBaudCount(uint32_t value)
{
	if (value > 0)
	{
		HAL_UART_ReceiverTimeout_Config(&_uart_handle, value);
		HAL_UART_EnableReceiverTimeout(&_uart_handle);
	}
	else
	{
		HAL_UART_DisableReceiverTimeout(&_uart_handle);
	}
}

int32_t bsp::Serial::HaveRead()
{
	return _uart_handle.RxXferSize - _rx_dma_channel->RemainingUntransmittedBytes();
}

/* #region 被中断处理函数回调的函数 */

void bsp::Serial::OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos)
{
	bsp::Serial::Instance()._receiving_completion_signal->ReleaseFromISR();
}

void bsp::Serial::OnSendCompleteCallback(UART_HandleTypeDef *huart)
{
	bsp::Serial::Instance()._sending_completion_signal->ReleaseFromISR();
}

void bsp::Serial::OnReadTimeout(UART_HandleTypeDef *huart)
{
	Serial::Instance()._receiving_completion_signal->ReleaseFromISR();
}

/* #endregion */

/* #region 读写冲关 */

int32_t bsp::Serial::Read(base::Span const &span)
{
	if (span.Size() > UINT16_MAX)
	{
		throw std::invalid_argument{"count 太大"};
	}

	base::LockGuard l{*_read_lock};
	while (true)
	{
		{
			bsp::di::interrupt::GlobalInterruptGuard g;

			// HAL_UART_Receive_DMA
			// HAL_UARTEx_ReceiveToIdle_DMA
			HAL_UART_Receive_DMA(&_uart_handle, span.Buffer(), span.Size());

			/*
			 * 通过赋值为空指针，把传输半满回调给禁用，不然接收的数据较长，超过缓冲区一半时，
			 * 即使是一次性接收的，UART 也会回调 OnReceiveEventCallback 两次。
			 *
			 * 这个操作需要在临界区中，并且 DMA 的中断要处于 freertos 的管理范围内，否则无效。
			 */
			_uart_handle.hdmarx->XferHalfCpltCallback = nullptr;
		}

		_receiving_completion_signal->Acquire();
		if (HaveRead() > 0)
		{
			return HaveRead();
		}
	}
}

void bsp::Serial::Write(base::ReadOnlySpan const &span)
{
	_sending_completion_signal->Acquire();
	HAL_StatusTypeDef ret = HAL_UART_Transmit_DMA(&_uart_handle,
												  span.Buffer(),
												  span.Size());

	if (ret != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"发送失败"};
	}
}

void bsp::Serial::Close()
{
	HAL_UART_DMAStop(&_uart_handle);
	bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::USART1_IRQn));
	bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn));
	bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn));
	_is_open = false;
}

/* #endregion */

bsp::Serial &bsp::Serial::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<Serial>
	{
	public:
		std::unique_ptr<Serial> Create() override
		{
			return std::unique_ptr<Serial>{new bsp::Serial{}};
		}
	};

	Getter g;
	return g.Instance();
}

void bsp::Serial::Open(base::serial::Direction direction,
					   base::serial::BaudRate const &baud_rate,
					   base::serial::DataBits const &data_bits,
					   base::serial::Parity parity,
					   base::serial::StopBits stop_bits,
					   base::serial::HardwareFlowControl hardware_flow_control)
{
	_direction = direction;
	_baud_rate = baud_rate.Value();
	_data_bits = data_bits.Value();
	_parity = parity;
	_stop_bits = stop_bits;
	_hardware_flow_control = hardware_flow_control;

	if (_is_open)
	{
		return;
	}

	_is_open = true;

	/*
	 * 先立刻释放一次信号量，等会 Write 方法被调用时直接通过，不被阻塞。
	 * 然后在发送完成之前，第二次 Write 就会被阻塞了，这还能防止 Write
	 * 被多线程同时调用。
	 */
	_sending_completion_signal->Release();
	InitializeGpio();
	InitializeDma();
	InitializeUart();
	InitializeInterrupt();
}

namespace
{
	class Collection
	{
	private:
		void Add(base::serial::ISerial *o)
		{
			_dic.Add(o->Name(), o);
		}

	public:
		Collection()
		{
			Add(&bsp::Serial::Instance());
		}

		base::Dictionary<std::string, base::serial::ISerial *> _dic{};
	};

	class Getter :
		public bsp::TaskSingletonGetter<Collection>
	{
	public:
		std::unique_ptr<Collection> Create() override
		{
			return std::unique_ptr<Collection>{new Collection{}};
		}
	};
} // namespace

base::serial::ISerial &base::serial::MainSerial()
{
	return bsp::Serial::Instance();
}

base::IDictionary<std::string, base::serial::ISerial *> const &base::serial::SerialCollection()
{
	Getter g;
	return g.Instance()._dic;
}
