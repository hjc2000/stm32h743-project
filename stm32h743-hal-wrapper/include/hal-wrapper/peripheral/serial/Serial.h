#pragma once
#include <base/HandleWrapper.h>
#include <bsp-interface/serial/ISerial.h>
#include <hal-wrapper/peripheral/uart/UartConfig.h>
#include <task/BinarySemaphore.h>
#include <task/Critical.h>
#include <task/Mutex.h>

extern "C"
{
	void USART1_IRQHandler();
	void DMA_STR0_IRQHandler();
	void DMA_STR1_IRQHandler();
}

namespace hal
{
	class Serial
		: public bsp::ISerial,
		  public base::HandleWrapper<UART_HandleTypeDef>
	{
	private:
		Serial() = default;

#pragma region 属性的字段
		uint32_t _baud_rate = 115200;
		uint8_t _data_bits = 8;
		bsp::ISerial::ParityOption _parity = bsp::ISerial::ParityOption::None;
		bsp::ISerial::StopBitsOption _stop_bits = bsp::ISerial::StopBitsOption::One;
		bsp::ISerial::HardwareFlowControlOption _hardware_flow_control = bsp::ISerial::HardwareFlowControlOption::None;
#pragma endregion

		bool _have_begun = false;
		UART_HandleTypeDef _uart_handle{};
		DMA_HandleTypeDef _tx_dma_handle{};
		DMA_HandleTypeDef _rx_dma_handle{};
		task::BinarySemaphore _send_complete_signal;
		task::BinarySemaphore _receive_complete_signal;
		task::Mutex _read_lock{};

		/// @brief 通过串口句柄和 DMA 寄存器，获取当前 DMA 接收了多少个字节。
		/// @return
		int32_t HaveRead();

		friend void ::USART1_IRQHandler();
		friend void ::DMA_STR0_IRQHandler();
		friend void ::DMA_STR1_IRQHandler();
		static void OnMspInitCallback(UART_HandleTypeDef *huart);

#pragma region 被中断处理函数回调的函数
		static void OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos);
		static void OnSendCompleteCallback(UART_HandleTypeDef *huart);
		static void OnReadTimeout(UART_HandleTypeDef *huart);
#pragma endregion

	public:
		static Serial &Instance()
		{
			static Serial o;
			return o;
		}

		UART_HandleTypeDef &Handle() override;

#pragma region Stream
		/// <summary>
		///		调用后临时启动 DMA 接收一次数据。
		///		* 本类没有缓冲机制，所以上层应用如果调用 Read 不及时，会丢失数据。
		///		* 因为调用一次 Read 具有一定开销，需要设置寄存器，使能中断，设置一些
		///		  状态变量。所以为了提高效率，每次调用 Read 时传入的 buffer 适当大一些，
		///		  并且 count 大一些。
		///		* 没有超时机制。这是将串口实现为 Stream 并且利用 DMA 进行接收的代价。
		///		  对于 modbus，如果接收了一个 modbus 帧的一部分后，继续接收，会一直等到
		///		  上位机发数据过来为止，即使上位机中断旧的帧，开始了新的帧。这就需要在接收
		///		  剩余部分前，记录当前时间，然后调用 Read 函数继续接收剩余部分。等到 Read
		///		  返回后，再查看一下时间，与刚才记录的时间进行比较，看是否超时。如果超时了，
		///		  就说明这次接收到的不是旧的 modbus 帧的剩余部分，而是新的 modbus 帧。
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="offset"></param>
		/// <param name="count"></param>
		/// <returns></returns>
		int32_t Read(uint8_t *buffer, int32_t offset, int32_t count) override;

		/// @brief 调用后临时启动 DMA 进行一次发送。
		/// @param buffer
		/// @param offset
		/// @param count
		void Write(uint8_t const *buffer, int32_t offset, int32_t count) override;

		void Close() override;
#pragma endregion

#pragma region 属性
		uint32_t BaudRate() const override;
		void SetBaudRate(uint32_t value) override;

		uint8_t DataBits() const override;
		void SetDataBits(uint8_t value) override;

		bsp::ISerial::ParityOption Parity() const override;
		void SetParity(bsp::ISerial::ParityOption value) override;

		bsp::ISerial::StopBitsOption StopBits() const override;
		void SetStopBits(bsp::ISerial::StopBitsOption value) override;

		bsp::ISerial::HardwareFlowControlOption HardwareFlowControl() const override;
		void SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption value) override;
#pragma endregion

		void SetReadTimeoutByBaudCount(uint32_t value);
		void SetReadTimeoutByFrameCount(uint32_t value);

		/// @brief 启动串口。
		/// @note 本函数幂等，调用后，启动串口，再次调用会直接返回，只有调用 Close
		/// 后才能重新启动串口。
		/// @note 本函数不是线程安全和可重入的，包括实现幂等的机制也不是线程安全和可重入的。
		void Open() override;
	};
}
