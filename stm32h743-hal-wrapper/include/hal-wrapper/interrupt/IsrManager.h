#pragma once
#include<bsp-interface/interrupt/IsrManager.h>

namespace hal
{
	/// <summary>
	///		获取中断服务程序管理器。
	/// </summary>
	/// <returns></returns>
	bsp::IsrManager &GetIsrManager();
}