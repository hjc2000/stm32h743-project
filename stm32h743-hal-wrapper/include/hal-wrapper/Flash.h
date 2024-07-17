#pragma once
#include <atomic>
#include <base/LockGuard.h>
#include <hal.h>
#include <task/BinarySemaphore.h>

extern "C"
{
	void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
	void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);
}

namespace hal
{
	class Flash
		: public base::ILock
	{
	private:
		Flash();

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

		/// @brief 将相对于指定 bank 的起始地址的地址转化为绝对地址。
		/// @tparam RetPtrType 返回的指针的类型。
		/// @param bank_id 相对于哪一个 bank 的起始地址？
		/// @param addr 相对于此 bank 起始地址的地址。
		/// @return 返回可以用来直接解引用从而操作该地址的指针。
		template <typename RetPtrType>
		RetPtrType *GetAbsoluteAddress(int32_t bank_id, size_t addr)
		{
			size_t base_addr;
			switch (bank_id)
			{
			case 1:
			{
				if (addr >= Bank1Size())
				{
					throw std::out_of_range{"地址超出范围"};
				}

				base_addr = Bank1BaseAddress();
				break;
			}
			case 2:
			{
				if (addr >= Bank2Size())
				{
					throw std::out_of_range{"地址超出范围"};
				}

				base_addr = Bank2BaseAddress();
				break;
			}
			default:
			{
				throw std::invalid_argument{"非法 bank_id"};
			}
			}

			return reinterpret_cast<RetPtrType *>(base_addr + addr);
		}
#pragma endregion

		static uint32_t SectorIndexToDefine(int32_t index);
		friend void ::HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
		friend void ::HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);

		std::atomic_bool _operation_failed = false;
		task::BinarySemaphore _operation_completed;

	public:
		static Flash &Instance()
		{
			static Flash o;
			return o;
		}

		void Lock() override;
		void Unlock() override;

		/// @brief 擦除一整个 bank。
		/// @note stm32h743 有 2 个 bank。典型用法是：bank1 用来存放程序，bank2 用来存放数据。
		/// @param bank_id 要擦除的扇区的 id。例如要擦除 bank1，就传入 1，要擦除 bank2 就传入 2.
		void EraseBank(int32_t bank_id);

		/// @brief 擦除指定 bank 中从 start_sector_index 开始的 sector_count 个扇区。
		/// @param bank_id 要擦除的扇区所在的 bank 的 id。
		/// @param start_sector_index 要擦除的扇区的起始索引。
		/// @param sector_count 要擦除的扇区的数量。
		void EraseSector(int32_t bank_id, int32_t start_sector_index, int32_t sector_count);

		/// @brief 读取指定 bank 的指定地址的 32 位数据
		/// @param bank_id bank 的 id。例如 bank1 的 id 是 1.
		/// @param addr 相对于此 bank 的起始地址的地址。
		/// @return 该地址的数据。
		uint32_t ReadUInt32(int32_t bank_id, size_t addr);

		/// @brief 对齐地写 bank
		/// @param bank_id 要写入的 bank 的 id.
		///
		/// @param addr 要写入的数据相对于此 bank 的起始地址的地址。
		/// @warning 此地址必须能被 32 整除。
		///
		/// @param buffer 要写入的数据所在的缓冲区。
		/// @param count 要写入的字节数。
		void WriteInAlignment(int32_t bank_id, size_t addr, uint8_t *buffer, int32_t count);
	};
}
