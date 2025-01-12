#include <AtkKey.h>
#include <base/container/Dictionary.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/key.h>
#include <bsp-interface/TaskSingletonGetter.h>

namespace
{
	class Initializer
	{
	public:
		Initializer()
		{
			AddKey(&bsp::Key0::Instance());
			AddKey(&bsp::Key1::Instance());
		}

		void AddKey(bsp::IKey *key)
		{
			_collection.Add(key->KeyName(), key);
		}

		base::Dictionary<std::string, bsp::IKey *> _collection;
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

base::IDictionary<std::string, bsp::IKey *> const &DI_KeyCollection()
{
	Getter g;
	return g.Instance()._collection;
}
