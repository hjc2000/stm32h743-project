#include "Flash.h"
#include <hal-wrapper/interrupt/Interrupt.h>
#include <stdexcept>

hal::Flash::Flash()
{
	hal::Interrupt::EnableIRQ(IRQn_Type::FLASH_IRQn);
}

uint32_t hal::Flash::SectorIndexToDefine(int32_t index)
{
	switch (index)
	{
	case 0:
	{
		return FLASH_SECTOR_0;
	}
	case 1:
	{
		return FLASH_SECTOR_1;
	}
	case 2:
	{
		return FLASH_SECTOR_2;
	}
	case 3:
	{
		return FLASH_SECTOR_3;
	}
	case 4:
	{
		return FLASH_SECTOR_4;
	}
	case 5:
	{
		return FLASH_SECTOR_5;
	}
	case 6:
	{
		return FLASH_SECTOR_6;
	}
	case 7:
	{
		return FLASH_SECTOR_7;
	}
	default:
	{
		throw std::out_of_range{"index 超出范围"};
	}
	}
}

void hal::Flash::Lock()
{
	HAL_StatusTypeDef result = HAL_FLASH_Lock();
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"锁定 flash 失败"};
	}
}

void hal::Flash::Unlock()
{
	HAL_StatusTypeDef result = HAL_FLASH_Unlock();
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"解锁 flash 失败"};
	}
}

size_t hal::Flash::GetBankBaseAddress(int32_t bank_id) const
{
	switch (bank_id)
	{
	case 1:
	{
		return 0x08000000;
	}
	case 2:
	{
		return 0x08100000;
	}
	default:
	{
		throw std::invalid_argument{"非法 bank_id"};
	}
	}
}

size_t hal::Flash::GetBankSize(int32_t bank_id) const
{
	switch (bank_id)
	{
	case 1:
	{
		return 0x080fffff - GetBankBaseAddress(1) + 1;
	}
	case 2:
	{
		return 0x081fffff - GetBankBaseAddress(2) + 1;
	}
	default:
	{
		throw std::invalid_argument{"非法 bank_id"};
	}
	}
}

#pragma region 擦除
void hal::Flash::EraseBank(int32_t bank_id)
{
	FLASH_EraseInitTypeDef def;

	// 擦除类型为 bank 擦除
	def.TypeErase = FLASH_TYPEERASE_MASSERASE;

	// 选择 bank
	switch (bank_id)
	{
	case 1:
	{
		def.Banks = FLASH_BANK_1;
		break;
	}
	case 2:
	{
		def.Banks = FLASH_BANK_2;
		break;
	}
	default:
	{
		throw std::invalid_argument{"非法 bank_id"};
	}
	}

	// 每次擦除 64 位，直到擦除整个 bank。越高的电压擦除时并行的位数就越多。
	def.VoltageRange = FLASH_VOLTAGE_RANGE_4;

	/* 擦除到某个扇区的时候如果发生错误，会停止擦除，将扇区索引赋值给 error_sector_index，
	 * 然后返回。
	 */
	HAL_StatusTypeDef result = HAL_FLASHEx_Erase_IT(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动擦除流程失败"};
	}

	_operation_completed.Acquire();
	if (_operation_failed)
	{
		throw std::runtime_error{"擦除流程结束，出错了"};
	}

	SCB_CleanInvalidateDCache();
}

void hal::Flash::EraseSector(int32_t bank_id, int32_t sector_index)
{
	FLASH_EraseInitTypeDef def;

	// 擦除类型为扇区擦除
	def.TypeErase = FLASH_TYPEERASE_SECTORS;

	// 选择 bank
	switch (bank_id)
	{
	case 1:
	{
		def.Banks = FLASH_BANK_1;
		break;
	}
	case 2:
	{
		def.Banks = FLASH_BANK_2;
		break;
	}
	default:
	{
		throw std::invalid_argument{"非法 bank_id"};
	}
	}

	def.Sector = SectorIndexToDefine(sector_index);
	def.NbSectors = 1;

	// 每次擦除 64 位，直到擦除整个 bank。越高的电压擦除时并行的位数就越多。
	def.VoltageRange = FLASH_VOLTAGE_RANGE_4;

	/* 擦除到某个扇区的时候如果发生错误，会停止擦除，将扇区索引赋值给 error_sector_index，
	 * 然后返回。
	 */
	HAL_StatusTypeDef result = HAL_FLASHEx_Erase_IT(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动擦除流程失败"};
	}

	_operation_completed.Acquire();
	if (_operation_failed)
	{
		throw std::runtime_error{"擦除流程结束，出错了"};
	}

	SCB_CleanInvalidateDCache();
}

#pragma endregion

void hal::Flash::ReadBuffer(int32_t bank_id, size_t addr, uint8_t *buffer, int32_t count)
{
	uint8_t *absolute_address = reinterpret_cast<uint8_t *>(GetAbsoluteAddress(bank_id, addr));
	std::copy(absolute_address, absolute_address + count, buffer);
}

void hal::Flash::Program(int32_t bank_id, size_t addr, uint8_t const *buffer)
{
	if (addr % MinProgrammingUnit() != 0)
	{
		throw std::invalid_argument{"addr 必须 32 字节对齐，即要能被 32 整除"};
	}

	if (reinterpret_cast<size_t>(buffer) % 4 != 0)
	{
		throw std::invalid_argument{"buffer 必须 4 字节对齐，即要能被 4 整除"};
	}

	HAL_StatusTypeDef result = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_FLASHWORD,
													static_cast<uint32_t>(GetAbsoluteAddress(bank_id, addr)),
													reinterpret_cast<uint32_t>(buffer));

	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动编程时发生错误"};
	}

	_operation_completed.Acquire();
	if (_operation_failed)
	{
		throw std::runtime_error{"擦除流程结束，出错了"};
	}

	SCB_CleanInvalidateDCache();
}

extern "C"
{
	void FLASH_IRQHandler()
	{
		HAL_FLASH_IRQHandler();
	}

	void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue)
	{
		hal::Flash::Instance()._operation_failed = false;
		hal::Flash::Instance()._operation_completed.ReleaseFromISR();
	}

	void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
	{
		hal::Flash::Instance()._operation_failed = true;
		hal::Flash::Instance()._operation_completed.ReleaseFromISR();
	}
}
