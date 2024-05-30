#pragma once
#include<hal.h>

namespace hal
{
	class DmaConfig
	{
	public:
		enum class Request
		{
			MemoryToMemory = DMA_REQUEST_MEM2MEM,
			Adc1 = DMA_REQUEST_ADC1,
			Adc2 = DMA_REQUEST_ADC2,
			Usart1Rx = DMA_REQUEST_USART1_RX,
			Usart1Tx = DMA_REQUEST_USART1_TX,
			Usart2Rx = DMA_REQUEST_USART2_RX,
			Usart2Tx = DMA_REQUEST_USART2_TX,
			Usart3Rx = DMA_REQUEST_USART3_RX,
			Usart3Tx = DMA_REQUEST_USART3_TX,
		};

		Request _request;

		enum class Direction
		{
			PeripheralToMemory = DMA_PERIPH_TO_MEMORY,
			MemoryToPeripheral = DMA_MEMORY_TO_PERIPH,
			MemoryToMemory = DMA_MEMORY_TO_MEMORY,
		};

		Direction _direction;

		enum class PeripheralAddressIncrease
		{
			Enable = DMA_PINC_ENABLE,
			Disable = DMA_PINC_DISABLE,
		};

		PeripheralAddressIncrease _peripheral_address_increase;

		enum class MemoryAddressIncrease
		{
			Enable = DMA_MINC_ENABLE,
			Disable = DMA_MINC_DISABLE,
		};

		MemoryAddressIncrease _memory_address_increase;

		enum class PeripheralDataAlignment
		{
			Byte = DMA_PDATAALIGN_BYTE,

			/// <summary>
			///		16 位
			/// </summary>
			HalfWord = DMA_PDATAALIGN_HALFWORD,

			/// <summary>
			///		32 位
			/// </summary>
			Word = DMA_PDATAALIGN_WORD,
		};

		PeripheralDataAlignment _peripheral_data_alignment;

		enum class MemoryDataAlignment
		{
			Byte = DMA_MDATAALIGN_BYTE,

			/// <summary>
			///		16 位
			/// </summary>
			HalfWord = DMA_MDATAALIGN_HALFWORD,

			/// <summary>
			///		32 位
			/// </summary>
			Word = DMA_MDATAALIGN_WORD,
		};

		MemoryDataAlignment _memory_data_alignment;

		enum class Mode
		{
			Normal = DMA_NORMAL,
			Circular = DMA_CIRCULAR,
			PfCtrl = DMA_PFCTRL,
			DoubleBufferM0 = DMA_DOUBLE_BUFFER_M0,
			DoubleBufferM1 = DMA_DOUBLE_BUFFER_M1,
		};

		Mode _mode;

		enum class Priority
		{
			Low = DMA_PRIORITY_LOW,
			Medium = DMA_PRIORITY_MEDIUM,
			High = DMA_PRIORITY_HIGH,
			VeryHigh = DMA_PRIORITY_VERY_HIGH,
		};

		Priority _priority;

		uint32_t FIFOMode;             /*!< Specifies if the FIFO mode or Direct mode will be used for the specified stream.
		This parameter can be a value of @ref DMA_FIFO_direct_mode
		@note The Direct mode (FIFO mode disabled) cannot be used if the
		memory-to-memory data transfer is configured on the selected stream       */

		uint32_t FIFOThreshold;        /*!< Specifies the FIFO threshold level.
		This parameter can be a value of @ref DMA_FIFO_threshold_level                  */

		uint32_t MemBurst;             /*!< Specifies the Burst transfer configuration for the memory transfers.
		It specifies the amount of data to be transferred in a single non interruptible
		transaction.
		This parameter can be a value of @ref DMA_Memory_burst
		@note The burst mode is possible only if the address Increment mode is enabled. */

		uint32_t PeriphBurst;          /*!< Specifies the Burst transfer configuration for the peripheral transfers.
		It specifies the amount of data to be transferred in a single non interruptible
		transaction.
		This parameter can be a value of @ref DMA_Peripheral_burst
		@note The burst mode is possible only if the address Increment mode is enabled. */
	};
}