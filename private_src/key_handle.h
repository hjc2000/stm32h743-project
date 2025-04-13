#pragma once
#include "base/peripheral/key/key_handle.h"

class base::key::key_handle
{
public:
	///
	/// @brief 按键处于被按下的状态。
	///
	/// @note 此函数内部不会进行按键消抖，抖动会引起此函数返回值的变化。
	///
	/// @return 按键处于被按下的状态则返回 true，按键不处于被按下的状态则返回 false。
	///
	virtual bool KeyIsDown() = 0;
};
