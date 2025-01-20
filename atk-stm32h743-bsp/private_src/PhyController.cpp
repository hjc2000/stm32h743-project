#include "PhyController.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>

uint32_t bsp::PhyController::ReadRegister(uint32_t register_index)
{
	return _controller->ReadPHYRegister(register_index);
}

void bsp::PhyController::WriteRegister(uint32_t register_index, uint32_t value)
{
	_controller->WritePHYRegister(register_index, value);
}

void bsp::PhyController::HardwareReset()
{
	/* 公司的开发板是旧版的，复位需要先输出高电平，延时后输出低电平。
	 * 家里的开发板是新版的，复位需要先输出低电平，延时后输出高电平。
	 */

	// 硬件复位
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
	bsp::di::Delayer().Delay(std::chrono::milliseconds{100});

	// 复位结束
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
	bsp::di::Delayer().Delay(std::chrono::milliseconds{100});
}
