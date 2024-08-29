#include <AtkKey.h>
#include <base/container/Collection.h>
#include <base/Initializer.h>
#include <bsp-interface/di/key.h>
#include <bsp-interface/key/KeyScanner.h>

static base::Initializer _initializer{
    []()
    {
        DI_KeyCollection();
        DI_KeyScanner();
    }};

class Collection
{
public:
    Collection()
    {
        AddKey(&bsp::Key0::Instance());
        AddKey(&bsp::Key1::Instance());
    }

    base::Collection<std::string, bsp::IKey *> _collection;

    void AddKey(bsp::IKey *key)
    {
        _collection.Put(key->KeyName(), key);
    }
};

base::ICollection<std::string, bsp::IKey *> const &DI_KeyCollection()
{
    static Collection o;
    return o._collection;
}

bsp::IKeyScanner &DI_KeyScanner()
{
    static bsp::KeyScanner key_scanner{};
    return key_scanner;
}
