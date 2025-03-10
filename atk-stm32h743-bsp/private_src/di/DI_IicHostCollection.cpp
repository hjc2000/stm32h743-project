#include <base/container/Dictionary.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/iic/GpioSoftwareIicHost.h>
#include <bsp-interface/iic/MutexIicHost.h>
#include <bsp-interface/TaskSingletonGetter.h>

namespace
{
	class Initializer
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
		Initializer()
		{
			Add(_mutex_gpio_iic_host);
		}

		base::Dictionary<std::string, bsp::IIicHost *> _dic;
	};

	class Getter :
		public bsp::TaskSingletonGetter<Initializer>
	{
	public:
		std::unique_ptr<Initializer> Create() override
		{
			return std::unique_ptr<Initializer>{new Initializer{}};
		}
	};
} // namespace

base::IDictionary<std::string, bsp::IIicHost *> const &DI_IicHostCollection()
{
	Getter o;
	return o.Instance()._dic;
}
