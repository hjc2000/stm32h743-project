#include "base/define.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/iic/GpioSoftwareIicHost.h>
#include <bsp-interface/iic/MutexIicHost.h>

namespace
{
	class DictionaryProvider
	{
	private:
		/// @brief 连接着 EEROM 芯片和 PCF8574T 芯片的 GPIO 模拟 IIC 主机接口。
		bsp::GpioSoftwareIicHost _gpio_iic_host{"gpio_iic_host", "PH4", "PH5"};
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

PREINIT(DI_IicHostCollection)

base::IDictionary<std::string, bsp::IIicHost *> const &DI_IicHostCollection()
{
	static DictionaryProvider o;
	return o._dic;
}
