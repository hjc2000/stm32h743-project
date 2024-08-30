#include "Dma1Stream0.h"
#include "Dma1Stream1.h"
#include "DmaOptions.h"
#include <base/container/Collection.h>
#include <base/SingletonGetter.h>
#include <bsp-interface/di/dma.h>
#include <bsp-interface/di/interrupt.h>
#include <map>

std::shared_ptr<bsp::IDmaOptions> DICreate_DmaOptions()
{
    return std::shared_ptr<bsp::IDmaOptions>{new bsp::DmaOptions{}};
}

base::ICollection<std::string, bsp::IDmaChannel *> const &DI_DmaChannelCollection()
{
    class Initializer
    {
    private:
        Initializer()
        {
            Add(&bsp::Dma1Stream0::Instance());
            Add(&bsp::Dma1Stream1::Instance());
        }

        void Add(bsp::IDmaChannel *o)
        {
            _collection.Put(o->Name(), o);
        }

    public:
        base::Collection<std::string, bsp::IDmaChannel *> _collection;

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
