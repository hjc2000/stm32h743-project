#include "base/container/Dictionary.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/embedded/iic/IicHost.h"
#include "bsp-interface/di/expanded_io.h"
#include "bsp-interface/expanded_io/PCF8574.h"

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

	class DictionaryProvider
	{
	private:
		bsp::PCF8574 _ex_io{
			"ex_io",
			base::gpio::GpioPin{base::gpio::PortEnum::PortB, 12},
			base::iic::iic_host_slot[0],
			0,
		};

		void Add(bsp::IExpandedIoPort &port)
		{
			_dic.Add(port.Name(), &port);
		}

	public:
		DictionaryProvider()
		{
			Add(_ex_io);
		}

		base::Dictionary<std::string, bsp::IExpandedIoPort *> _dic;
	};

} // namespace

// PREINIT(DI_ExpandedIoPortCollection)

base::IDictionary<std::string, bsp::IExpandedIoPort *> const &DI_ExpandedIoPortCollection()
{
	static DictionaryProvider o;
	return o._dic;
}
