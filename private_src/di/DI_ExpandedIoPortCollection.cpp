#include "base/container/Dictionary.h"
#include "base/embedded/iic/IicHost.h"
#include "bsp-interface/di/expanded_io.h"
#include "bsp-interface/di/gpio.h"
#include "bsp-interface/expanded_io/PCF8574.h"
#include <memory>

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
		/// @brief 连接着 EEROM 芯片和 PCF8574T 芯片的 GPIO 模拟 IIC 主机接口。
		std::shared_ptr<base::iic::SoftwareIicHostPinDriver<base::gpio::GpioPin>> _pin_driver{new base::iic::SoftwareIicHostPinDriver<base::gpio::GpioPin>{
			base::gpio::GpioPin{base::gpio::PortEnum::PortH, 4},
			base::gpio::GpioPin{base::gpio::PortEnum::PortH, 5},
		}};

		bsp::PCF8574 _ex_io{
			"ex_io",
			DI_GpioPinCollection().Get("PB12"),
			std::shared_ptr<base::iic::IicHost>{new base::iic::IicHost{_pin_driver}},
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
