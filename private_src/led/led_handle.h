#pragma once
#include "base/peripheral/led/led_handle.h"

class base::led::led_handle
{
public:
	/* #region 控制 LED 开关 */

	///
	/// @brief 打开 LED.
	///
	///
	virtual void TurnOn() = 0;

	///
	/// @brief 关闭 LED.
	///
	///
	virtual void TurnOff() = 0;

	///
	/// @brief 翻转 LED.
	///
	///
	virtual void Toggle() = 0;

	/* #endregion */

	///
	/// @brief 获取 LED 灯的状态。
	///
	/// @return base::led::State
	///
	virtual base::led::State State() const = 0;
};
