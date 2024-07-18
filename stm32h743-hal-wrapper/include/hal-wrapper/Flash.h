#pragma once
#include <array>
#include <atomic>
#include <base/LockGuard.h>
#include <bsp-interface/flash/IFlash.h>
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
		: public bsp::IFlash
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
		/// @param bank_id 相对于哪一个 bank 的起始地址？
		/// @param addr 相对于此 bank 起始地址的地址。
		/// @return 绝对地址。可以被强制转换为指针。
		size_t GetAbsoluteAddress(int32_t bank_id, size_t addr)
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

			return base_addr + addr;
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

		/// @brief flash 的最小编程单位。单位：字节。
		/// @note 最小单位是一次编程必须写入这么多字节，即使要写入的数据没有这么多，在一次
		/// 写入后，整个单位大小的区域都无法再次写入了，除非擦除整个扇区。
		/// @return 返回此 flash 编程的最小单位。
		int32_t MinProgrammingUnit() override
		{
			return 32;
		}

		/// @brief flash 的地址需要对齐到的字节数。本类中其他方法，凡事要传入 flash 地址的，
		/// 都需要对齐到本属性。
		/// @return
		int32_t FlashAddressAlign() override
		{
			return 32;
		}

		/// @brief 擦除一整个 bank。
		/// @note stm32h743 有 2 个 bank。典型用法是：bank1 用来存放程序，bank2 用来存放数据。
		/// @param bank_id 要擦除的扇区的 id。例如要擦除 bank1，就传入 1，要擦除 bank2 就传入 2.
		void EraseBank(int32_t bank_id) override;

		/// @brief 擦除指定 bank 中从 start_sector_index 开始的 sector_count 个扇区。
		/// @param bank_id 要擦除的扇区所在的 bank 的 id。
		/// @param start_sector_index 要擦除的扇区的起始索引。
		/// @param sector_count 要擦除的扇区的数量。
		void EraseSector(int32_t bank_id, int32_t start_sector_index, int32_t sector_count) override;

		void ReadBuffer(int32_t bank_id, size_t addr, uint8_t *buffer, int32_t count) override;

		/// @brief 编程
		/// @param bank_id 要写入的 bank 的 id.
		///
		/// @param addr 要写入的数据相对于此 bank 的起始地址的地址。
		/// @warning 此地址要 32 字节对齐。
		///
		/// @param buffer 要写入到 flash 的数据所在的缓冲区。
		/// @warning buffer 的元素个数必须 >= MinProgrammingUnit，否则将发生内存访问越界。
		void Program(int32_t bank_id, size_t addr, uint32_t const *buffer) override;
	};
}
