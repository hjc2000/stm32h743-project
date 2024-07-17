#pragma once
#include <hal.h>

namespace hal
{
	class Flash
	{
	public:
		void Lock();
		void Unlock();

		/// @brief 擦除一整个 bank。
		/// @note stm32h743 有 2 个 bank。典型用法是：bank1 用来存放程序，bank2 用来存放数据。
		/// @param bank_id 要擦除的扇区的 id。例如要擦除 bank1，就传入 1，要擦除 bank2 就传入 2.
		void EraseBank(int32_t bank_id);

		/// @brief 读取指定地址的 32 位数据。
		/// @param addr flash 中要被读取的数据位于物理内存地址空间中的地址。
		/// @return 该地址的数据。
		uint32_t ReadUInt32(size_t addr);
	};
}
