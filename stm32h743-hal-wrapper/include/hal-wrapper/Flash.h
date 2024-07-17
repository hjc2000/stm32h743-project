#pragma once
#include <hal.h>

namespace hal
{
	class Flash
	{
	private:
		Flash() = default;

#pragma region 地址和大小
		static consteval size_t Bank1BaseAddress()
		{
			return 0x08000000;
		}

		static consteval size_t Bank1Size()
		{
			return 0x080fffff - Bank1BaseAddress() + 1;
		}

		static consteval size_t Bank2BaseAddress()
		{
			return 0x08100000;
		}

		static consteval size_t Bank2Size()
		{
			return 0x081fffff - Bank2BaseAddress() + 1;
		}
#pragma endregion

		static uint32_t SectorIndexToDefine(int32_t index);

	public:
		Flash &Instance()
		{
			static Flash o;
			return o;
		}

		void Lock();
		void Unlock();

		/// @brief 擦除一整个 bank。
		/// @note stm32h743 有 2 个 bank。典型用法是：bank1 用来存放程序，bank2 用来存放数据。
		/// @param bank_id 要擦除的扇区的 id。例如要擦除 bank1，就传入 1，要擦除 bank2 就传入 2.
		void EraseBank(int32_t bank_id);

		/// @brief 擦除指定 bank 中从 start_sector_index 开始的 sector_count 个扇区。
		/// @param bank_id 要擦除的扇区所在的 bank 的 id。
		/// @param start_sector_index 要擦除的扇区的起始索引。
		/// @param sector_count 要擦除的扇区的数量。
		void EraseSector(int32_t bank_id, int32_t start_sector_index, int32_t sector_count);

		/// @brief 读取指定地址的 32 位数据。
		/// @param addr 相对于此 flash 储存器起始地址的地址。
		/// @return 该地址的数据。
		uint32_t ReadUInt32(size_t addr);

		/// @brief 读取指定 bank 的指定地址的 32 位数据
		/// @param bank_id bank 的 id。例如 bank1 的 id 是 1.
		/// @param addr 相对于此 bank 的起始地址的地址。
		/// @return 该地址的数据。
		uint32_t ReadBankUInt32(int32_t bank_id, size_t addr);
	};
}
