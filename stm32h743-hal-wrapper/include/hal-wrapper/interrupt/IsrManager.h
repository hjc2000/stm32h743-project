#pragma once
#include <bsp-interface/interrupt/IsrManager.h>

namespace hal
{
	/// @brief 获取中断服务程序管理器。
	/// @return
	bsp::IsrManager &GetIsrManager();
}
