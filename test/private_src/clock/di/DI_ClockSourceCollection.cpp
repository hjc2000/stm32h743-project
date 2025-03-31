#include <base/container/Dictionary.h>
#include <bsp-interface/di/clock.h>
#include <HseClockSource.h>
#include <PllClockSource.h>

namespace
{
    class Initializer
    {
    public:
        Initializer()
        {
            _dic.Add(bsp::HseClockSource::Instance().Name(), &bsp::HseClockSource::Instance());
            _dic.Add(bsp::PllClockSource::Instance().Name(), &bsp::PllClockSource::Instance());
        }

        base::Dictionary<std::string, bsp::IClockSource *> _dic{};
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

base::IDictionary<std::string, bsp::IClockSource *> const &bsp::di::clock::ClockSourceCollection()
{
    Getter g;
    return g.Instance()._dic;
}
