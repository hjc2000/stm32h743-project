#include "Dma1Stream0.h"
#include "Dma1Stream1.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/dma.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <map>

namespace
{
    class Initializer
    {
    private:
        void Add(bsp::dma::IDmaChannel *o)
        {
            _dic.Add(o->Name(), o);
        }

    public:
        Initializer()
        {
            Add(&bsp::Dma1Stream0::Instance());
            Add(&bsp::Dma1Stream1::Instance());
        }

        base::Dictionary<std::string, bsp::dma::IDmaChannel *> _dic;
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

base::IDictionary<std::string, bsp::dma::IDmaChannel *> const &DI_DmaChannelCollection()
{
    Getter g;
    return g.Instance()._dic;
}
