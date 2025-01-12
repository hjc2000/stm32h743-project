#include <base/container/Dictionary.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/expanded_io/PCF8574.h>
#include <bsp-interface/TaskSingletonGetter.h>

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

	class Initializer
	{
	private:
		bsp::PCF8574 _ex_io{
			"ex_io",
			DI_GpioPinCollection().Get("PB12"),
			DI_IicHostCollection().Get("gpio_iic_host"),
			0,
		};

		void Add(bsp::IExpandedIoPort &port)
		{
			_dic.Add(port.Name(), &port);
		}

	public:
		Initializer()
		{
			Add(_ex_io);
		}

		base::Dictionary<std::string, bsp::IExpandedIoPort *> _dic;
	};

	class Getter :
		public base::SingletonGetter<Initializer>
	{
	public:
		std::unique_ptr<Initializer> Create() override
		{
			return std::unique_ptr<Initializer>{new Initializer{}};
		}

		void Lock() override
		{
			DI_DisableGlobalInterrupt();
		}

		void Unlock() override
		{
			DI_EnableGlobalInterrupt();
		}
	};
} // namespace

base::IDictionary<std::string, bsp::IExpandedIoPort *> const &DI_ExpandedIoPortCollection()
{
	Getter o;
	return o.Instance()._dic;
}
