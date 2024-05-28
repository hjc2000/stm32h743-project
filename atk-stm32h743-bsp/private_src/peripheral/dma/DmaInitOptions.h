#pragma once
#include<DmaEnum.h>

namespace bsp
{
	/// <summary>
	///		DMA 初始化选项。
	/// </summary>
	class DmaInitOptions
	{
	public:
		DmaInitOptions() = default;
		DmaInitOptions(DmaInitOptions const &o) = default;
		DmaInitOptions(DMA_InitTypeDef const &o);
		DmaInitOptions &operator=(DmaInitOptions const &o) = default;
		DmaInitOptions &operator=(DMA_InitTypeDef const &o);

		/// <summary>
		///		数据传输方向
		/// </summary>
		DmaDataTransferDirection _direction;

		/// <summary>
		///		一次传输结束后，外设地址是否应该递增。
		/// </summary>
		DmaPeripheralIncMode _peripheral_inc_mode;

		/// <summary>
		///		一次传输结束后，内存地址是否应该递增。
		/// </summary>
		DmaMemoryIncMode _mem_inc_mode;

		/// <summary>
		///		外设中的数据的对齐方式。
		/// </summary>
		PeripheralDataAlignment _peripheral_data_alignment;

		/// <summary>
		///		内存中的数据的对齐方式。
		/// </summary>
		MemoryDataAlignment _mem_data_alignment;

		/// <summary>
		///		数据传输模式。
		///		- 可以选择普通模式或循环模式。循环模式不能用于内存到内存传输模式。
		/// </summary>
		DmaMode _mode;

		/// <summary>
		///		此通道的传输优先级。
		/// </summary>
		DmaPriority _priority;

		operator DMA_InitTypeDef() const;
	};
}
