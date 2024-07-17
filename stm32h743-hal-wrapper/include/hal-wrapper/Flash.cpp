#include "Flash.h"
#include <stdexcept>

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

	uint32_t error_sector_index;

	/* 擦除到某个扇区的时候如果发生错误，会停止擦除，将扇区索引赋值给 error_sector_index，
	 * 然后返回。
	 */
	HAL_StatusTypeDef result = HAL_FLASHEx_Erase(&def, &error_sector_index);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		std::string msg = "擦除第 ";
		msg += std::to_string(error_sector_index);
		msg += " 个扇区时出错。";
		throw std::runtime_error{msg};
	}
}

uint32_t hal::Flash::ReadUInt32(size_t addr)
{
	if (addr >= Bank1Size() + Bank2Size())
	{
		throw std::out_of_range{"地址超出范围"};
	}

	volatile uint32_t *p = reinterpret_cast<volatile uint32_t *>(Bank1BaseAddress() + addr);
	return *p;
}

uint32_t hal::Flash::ReadBankUInt32(int32_t bank_id, size_t addr)
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

	volatile uint32_t *p = reinterpret_cast<volatile uint32_t *>(base_addr + addr);
	return *p;
}
