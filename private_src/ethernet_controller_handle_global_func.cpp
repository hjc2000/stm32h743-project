#include "base/embedded/ethernet/ethernet_controller_handle.h"
#include "base/embedded/extended_io/PCF8574.h"
#include "base/task/delay.h"
#include <chrono>

namespace
{
	/// @brief 开发板上的 PCF8574 各个引脚的功能。
	enum class ExIoFunctionEnum
	{
		/// @brief 蜂鸣器控制引脚
		BEEP_IO = 0,

		/// @brief AP3216C中断引脚
		AP_INT_IO = 1,

		/// @brief DCMI的电源控制引脚
		DCMI_PWDN_IO = 2,

		/// @brief USB电源控制引脚
		USB_PWR_IO = 3,

		/// @brief 用户自定义使用
		Costum = 4,

		/// @brief MPU9250中断引脚
		MPU_INT_IO = 5,

		/// @brief RS485_RE引脚
		RS485_RE_IO = 6,

		/// @brief 以太网复位引脚
		ETH_RESET_IO = 7,
	};

} // namespace

void base::ethernet::hardware_reset_phy(base::ethernet::ethernet_controller_handle &h)
{
	base::extended_io::PCF8574Operator pcf8574_operator{*base::extended_io::pcf8574_slot()[0]};

	/* 公司的开发板是旧版的，复位需要先输出高电平，延时后输出低电平。
	 * 家里的开发板是新版的，复位需要先输出低电平，延时后输出高电平。
	 */

	// 硬件复位
	pcf8574_operator.WriteBit(7, 0);
	base::task::Delay(std::chrono::milliseconds{100});

	// 复位结束
	pcf8574_operator.WriteBit(7, 1);
	base::task::Delay(std::chrono::milliseconds{100});
}
