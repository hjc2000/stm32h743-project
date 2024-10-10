#include <AtkKey.h>
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/key.h>
#include <bsp-interface/key/KeyScanner.h>

base::IDictionary<std::string, bsp::IKey *> const &DI_KeyCollection()
{
    class Initializer
    {
    private:
        Initializer()
        {
            AddKey(&bsp::Key0::Instance());
            AddKey(&bsp::Key1::Instance());
        }

        void AddKey(bsp::IKey *key)
        {
            _collection.Add(key->KeyName(), key);
        }

    public:
        base::Dictionary<std::string, bsp::IKey *> _collection;

        static Initializer &Instance()
        {
            class Getter : public base::SingletonGetter<Initializer>
            {
            public:
                std::unique_ptr<Initializer> Create() override
                {
                    return std::unique_ptr<Initializer>{new Initializer{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }
    };

    return Initializer::Instance()._collection;
}

bsp::IKeyScanner &DI_KeyScanner()
{
    class Getter : public base::SingletonGetter<bsp::KeyScanner>
    {
    public:
        std::unique_ptr<bsp::KeyScanner> Create() override
        {
            return std::unique_ptr<bsp::KeyScanner>{new bsp::KeyScanner{}};
        }

        void Lock() override
        {
            DI_InterruptSwitch().DisableGlobalInterrupt();
        }

        void Unlock() override
        {
            DI_InterruptSwitch().EnableGlobalInterrupt();
        }
    };

    Getter g;
    return g.Instance();
}
