#include "base/container/Dictionary.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/embedded/iic/SoftwareIicHostPinDriver.h"
#include "bsp-interface/di/iic.h"
#include "bsp-interface/iic/GpioSoftwareIicHost.h"
#include "bsp-interface/iic/MutexIicHost.h"
#include <memory>

namespace
{
	class DictionaryProvider
	{
	private:
		/// @brief 连接着 EEROM 芯片和 PCF8574T 芯片的 GPIO 模拟 IIC 主机接口。
		std::shared_ptr<base::iic::SoftwareIicHostPinDriver<base::gpio::GpioPin>> _pin_driver{new base::iic::SoftwareIicHostPinDriver<base::gpio::GpioPin>{
			base::gpio::GpioPin{base::gpio::PortEnum::PortH, 4},
			base::gpio::GpioPin{base::gpio::PortEnum::PortH, 5},
		}};

		bsp::GpioSoftwareIicHost _gpio_iic_host{"gpio_iic_host", _pin_driver};
		bsp::MutexIicHost _mutex_gpio_iic_host{&_gpio_iic_host};

		void Add(bsp::IIicHost &host)
		{
			_dic.Add(host.Name(), &host);
		}

	public:
		DictionaryProvider()
		{
			Add(_mutex_gpio_iic_host);
		}

		base::Dictionary<std::string, bsp::IIicHost *> _dic;
	};

} // namespace

base::IDictionary<std::string, bsp::IIicHost *> const &DI_IicHostCollection()
{
	static DictionaryProvider o;
	return o._dic;
}
