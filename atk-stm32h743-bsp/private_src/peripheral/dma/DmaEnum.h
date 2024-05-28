#pragma once
#include<hal.h>

namespace bsp
{
	enum class DmaDataTransferDirection
	{
		PeripheralToMemory = DMA_PERIPH_TO_MEMORY,
		MemoryToPeripheral = DMA_MEMORY_TO_PERIPH,
		MomoryToMemory = DMA_MEMORY_TO_MEMORY,
	};

	enum class DmaPeripheralIncMode
	{
		Enable = DMA_PINC_ENABLE,
		Disable = DMA_PINC_DISABLE,
	};

	enum class DmaMemoryIncMode
	{
		Enable = DMA_MINC_ENABLE,
		Disable = DMA_MINC_DISABLE,
	};

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

	enum class DmaMode
	{
		Normal = DMA_NORMAL,
		Circular = DMA_CIRCULAR,
	};

	enum class DmaPriority
	{
		Low = DMA_PRIORITY_LOW,
		Medium = DMA_PRIORITY_MEDIUM,
		High = DMA_PRIORITY_HIGH,
		VeryHigh = DMA_PRIORITY_VERY_HIGH,
	};
}
