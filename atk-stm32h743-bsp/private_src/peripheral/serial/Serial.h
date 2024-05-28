#pragma once
#include<UartInitOptions.h>
#include<atomic>
#include<bsp-interface/ISerial.h>
#include<hal.h>
#include<memory>
#include<task/BinarySemaphore.h>
#include<task/Critical.h>
#include<task/Mutex.h>

extern "C"
{
	void USART1_IRQHandler();
	void DMA1_Channel4_IRQHandler();
	void DMA1_Channel5_IRQHandler();
}

namespace bsp
{
	class Serial :
		public bsp::ISerial
	{
	private:
		Serial() = default;

		bool _have_begun = false;
		uint32_t _baud_rate = 115200;
		UART_HandleTypeDef _uart_handle{};
		DMA_HandleTypeDef _tx_dma_handle{};
		DMA_HandleTypeDef _rx_dma_handle{};
		USART_TypeDef *_uart_hardware_instance = USART1;
		DMA_Channel_TypeDef *_dma_channel_hardware_instance = DMA1_Channel4;
		task::BinarySemaphore _send_complete_signal;
		task::BinarySemaphore _receive_complete_signal;
		task::Mutex _read_lock{};
		int32_t _current_receive_count = 0;

		friend void ::USART1_IRQHandler();
		friend void ::DMA1_Channel4_IRQHandler();
		friend void ::DMA1_Channel5_IRQHandler();
		static void OnMspInitCallback(UART_HandleTypeDef *huart);

		#pragma region 被中断处理函数回调的函数
		static void OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos);
		static void OnSendCompleteCallback(UART_HandleTypeDef *huart);
		#pragma endregion

	public:
		static Serial &Instance()
		{
			static Serial o;
			return o;
		}

		#pragma region Stream
		bool CanRead() override;
		bool CanWrite() override;
		bool CanSeek() override;

		int64_t Length() override;
		void SetLength(int64_t value) override;

		/// <summary>
		///		调用后临时启动 DMA 接收一次数据。
		///		* 本类没有缓冲机制，所以上层应用如果调用 Read 不及时，会丢失数据。
		///		* 因为调用一次 Read 具有一定开销，需要设置寄存器，使能中断，设置一些
		///		  状态变量。所以为了提高效率，每次调用 Read 时传入的 buffer 适当大一些，
		///		  并且 count 大一些。
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="offset"></param>
		/// <param name="count"></param>
		/// <returns></returns>
		int32_t Read(uint8_t *buffer, int32_t offset, int32_t count) override;

		/// <summary>
		///		调用后临时启动 DMA 进行一次发送。
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="offset"></param>
		/// <param name="count"></param>
		void Write(uint8_t const *buffer, int32_t offset, int32_t count) override;

		void Flush() override;
		void Close() override;

		int64_t Position() override;
		void SetPosition(int64_t value) override;
		#pragma endregion

		/// <summary>
		///		启动串口。
		///		* 本函数幂等，调用后，启动串口，再次调用会直接返回，只有调用 Close
		///		  后才能重新启动串口。
		///		* 本函数不是线程安全和可重入的，包括实现幂等的机制也不是线程安全和可重入的。
		/// </summary>
		/// <param name="baud_rate">想要的波特率</param>
		void Begin(uint32_t baud_rate) override;
	};
}
