#include "base/embedded/ethernet/ethernet_controller_handle.h"
#include "base/task/delay.h"
#include "bsp-interface/di/expanded_io.h"
#include <chrono>

void base::ethernet::hardware_reset_phy(base::ethernet::ethernet_controller_handle &h)
{
	/* 公司的开发板是旧版的，复位需要先输出高电平，延时后输出低电平。
	 * 家里的开发板是新版的，复位需要先输出低电平，延时后输出高电平。
	 */

	// 硬件复位
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
	base::task::Delay(std::chrono::milliseconds{100});

	// 复位结束
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
	base::task::Delay(std::chrono::milliseconds{100});
}
