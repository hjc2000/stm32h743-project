#include "led_handle.h"
#include "base/embedded/led/LedBar.h"
#include "base/string/define.h"
#include "GreenDigitalLed.h"
#include "RedDigitalLed.h"
#include <memory>
#include <stdexcept>


/* #region open */

std::shared_ptr<base::led::led_handle> base::led::open(uint32_t led_id)
{
	switch (led_id)
	{
	case 0:
		{
			return std::shared_ptr<base::led::led_handle>{new bsp::RedDigitalLed{}};
		}
	case 1:
		{
			return std::shared_ptr<base::led::led_handle>{new bsp::GreenDigitalLed{}};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法 led_id."};
		}
	}
}

std::shared_ptr<base::led::led_handle> base::led::open(base::ui::Color color)
{
	switch (color)
	{
	case base::ui::Color::Red:
		{
			return std::shared_ptr<base::led::led_handle>{new bsp::RedDigitalLed{}};
		}
	case base::ui::Color::Green:
		{
			return std::shared_ptr<base::led::led_handle>{new bsp::GreenDigitalLed{}};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "没有该颜色的 LED."};
		}
	}
}

/* #endregion */

/* #region 控制 LED 开关 */

///
/// @brief 打开 LED.
///
/// @param h
///
void base::led::turn_on(base::led::led_handle &h)
{
	h.TurnOn();
}

///
/// @brief 关闭 LED.
///
/// @param h
///
void base::led::turn_off(base::led::led_handle &h)
{
	h.TurnOff();
}

///
/// @brief 翻转 LED.
///
/// @param h
///
void base::led::toggle(base::led::led_handle &h)
{
	h.Toggle();
}

/* #endregion */

base::led::State base::led::state(base::led::led_handle &h)
{
	return h.State();
}

void base::led::turn_on_error_led()
{
	if (base::led::led_bar().Count() > 0)
	{
		base::led::led_bar()[0].TurnOn();
	}
}
