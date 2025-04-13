#include "base/define.h"
#include "Key0.h"
#include "Key1.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/key.h>

namespace
{
	class DictionaryProvider
	{
	public:
		DictionaryProvider()
		{
			AddKey(&bsp::Key0::Instance());
			AddKey(&bsp::Key1::Instance());
		}

		void AddKey(bsp::IKey *key)
		{
			_dic.Add(key->KeyName(), key);
		}

		base::Dictionary<std::string, bsp::IKey *> _dic;
	};

} // namespace

PREINIT(bsp::di::key::KeyCollection)

base::IDictionary<std::string, bsp::IKey *> const &bsp::di::key::KeyCollection()
{
	static DictionaryProvider o;
	return o._dic;
}
