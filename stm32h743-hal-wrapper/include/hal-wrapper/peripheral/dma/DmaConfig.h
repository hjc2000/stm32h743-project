#pragma once
#include<hal.h>

namespace hal
{
	class DmaConfig
	{
	public:
		DmaConfig() = default;
		DmaConfig(DMA_InitTypeDef const &o);
		DmaConfig &operator=(DMA_InitTypeDef const &o);
		operator DMA_InitTypeDef();

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

		enum class FifoMode
		{
			Disable = DMA_FIFOMODE_DISABLE,
			Enable = DMA_FIFOMODE_ENABLE,
		};

		FifoMode _fifo_mode;

		enum class FifoThreshold
		{
			Threshold_1_div_4 = DMA_FIFO_THRESHOLD_1QUARTERFULL,
			Threshold_2_div_4 = DMA_FIFO_THRESHOLD_HALFFULL,
			Threshold_3_div_4 = DMA_FIFO_THRESHOLD_3QUARTERSFULL,
			Threshold_4_div_4 = DMA_FIFO_THRESHOLD_FULL,
		};

		FifoThreshold _fifo_threshold;

		enum class MemoryBurst
		{
			Single = DMA_MBURST_SINGLE,
			Inc4 = DMA_MBURST_INC4,
			Inc8 = DMA_MBURST_INC8,
			Inc16 = DMA_MBURST_INC16,
		};

		MemoryBurst _memory_burst;

		enum class PeripheralBurst
		{
			Single = DMA_PBURST_SINGLE,
			Inc4 = DMA_PBURST_INC4,
			Inc8 = DMA_PBURST_INC8,
			Inc16 = DMA_PBURST_INC16,
		};

		PeripheralBurst _peripheral_burst;
	};
}