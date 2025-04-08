#include "base/define.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/eerom.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/eerom/AT24C02_EEROM.h>

namespace
{
	class DictionaryProvider
	{
	private:
		bsp::AT24C02_EEROM _at24c02{"at24c02", DI_IicHostCollection().Get("gpio_iic_host")};

		void Add(bsp::IEEROM *eerom)
		{
			_dic.Add(eerom->Name(), eerom);
		}

	public:
		DictionaryProvider()
		{
			Add(&_at24c02);
		}

		base::Dictionary<std::string, bsp::IEEROM *> _dic;
	};

} // namespace

PREINIT(DI_EEROMCollection)

base::IDictionary<std::string, bsp::IEEROM *> const &DI_EEROMCollection()
{
	static DictionaryProvider o;
	return o._dic;
}
