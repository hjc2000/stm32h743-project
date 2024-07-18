#pragma once
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

		/// @brief 获取此 flash 的 bank 数量。
		/// @return
		int32_t BankCount() const override
		{
			return 2;
		}

		/// @brief 获取指定 bank 的扇区数量。
		/// @param bank_id
		/// @return
		int32_t GetBankSectorCount(int32_t bank_id) const override
		{
			return 8;
		}

		/// @brief 获取指定 bank 的基地址。
		/// @param bank_id
		/// @return
		size_t GetBankBaseAddress(int32_t bank_id) const override;

		/// @brief 获取指定 bank 的大小。单位：字节。
		/// @param bank_id
		/// @return
		virtual size_t GetBankSize(int32_t bank_id) const override;

		/// @brief flash 的最小编程单位。单位：字节。
		/// @note 最小单位是一次编程必须写入这么多字节，即使要写入的数据没有这么多，在一次
		/// 写入后，整个单位大小的区域都无法再次写入了，除非擦除整个扇区。
		/// @return 返回此 flash 编程的最小单位。
		int32_t MinProgrammingUnit() const override
		{
			return 32;
		}

		/// @brief 擦除一整个 bank。
		/// @note stm32h743 有 2 个 bank。典型用法是：bank1 用来存放程序，bank2 用来存放数据。
		/// @param bank_id 要擦除的扇区的 id。例如要擦除 bank1，就传入 1，要擦除 bank2 就传入 2.
		void EraseBank(int32_t bank_id) override;

		/// @brief 擦除指定 bank 的指定扇区。
		/// @param bank_id 要擦除的扇区所在的 bank
		/// @param sector_index 要擦除的扇区索引。
		void EraseSector(int32_t bank_id, int32_t sector_index);
		using bsp::IFlash::EraseSector;

		void ReadBuffer(int32_t bank_id, size_t addr, uint8_t *buffer, int32_t count) override;

		void Program(int32_t bank_id, size_t addr, uint8_t const *buffer) override;
	};
}
