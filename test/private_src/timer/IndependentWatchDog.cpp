#include "IndependentWatchDog.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/TaskSingletonGetter.h>

base::Hz bsp::IndependentWatchDog::InnerClockSourceFreq() const
{
    // 独立看门狗具有 40 kHz 的内部时钟。
    return base::Hz{40 * 1000};
}

bsp::IndependentWatchDog &bsp::IndependentWatchDog::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<IndependentWatchDog>
    {
    public:
        std::unique_ptr<IndependentWatchDog> Create() override
        {
            return std::unique_ptr<IndependentWatchDog>{new IndependentWatchDog{}};
        }
    };

    Getter g;
    return g.Instance();
}

void bsp::IndependentWatchDog::Open(std::chrono::milliseconds value)
{
    base::Seconds inner_clock_source_interval{InnerClockSourceFreq()};
    base::Seconds timeout{value};

    // 所需的 (分频器计数值 + 计数器计数值)
    int64_t total_count = static_cast<int64_t>(timeout / inner_clock_source_interval);

    // 让计数器重装载值尽量大，溢出了就增大分频系数
    for (uint16_t i = 2; i <= 8; i++)
    {
        // 从 2^2 = 4 开始，到 2^8 = 256，通过移位实现幂。i 代表的是 2 的幂
        uint16_t prescaler_value = static_cast<uint16_t>(1 << i);
        _config.SetReloadValue(total_count / prescaler_value);
        if (_config.ReloadValue() > 0X0FFF && i == 8)
        {
            // 最大分频和最大计数都无法表示这个时间，就按照能达到的最大值来。
            _config.SetReloadValue(0X0FFF);
            _config.SetPrescaler(bsp::IndependentWatchDogConfig_Prescaler::Div256);
            break;
        }

        if (_config.ReloadValue() <= 0x0FFF)
        {
            // i 代表的是 2 的幂，将 i 映射到分频系数枚举值
            _config.SetPrescalerByPow(i);
            break;
        }
    }

    _handle.Instance = HardwareInstance();
    _handle.Init = _config;
    HAL_IWDG_Init(&_handle);
}

void bsp::IndependentWatchDog::Close()
{
    bsp::di::Console().WriteError("看门狗一旦开启就无法关闭");
}

std::chrono::milliseconds bsp::IndependentWatchDog::Timeout() const
{
    base::Hz count_freq = InnerClockSourceFreq() / _config.GetPrescalerByUint32();
    base::Seconds count_period{count_freq};
    base::Seconds timeout = _config.ReloadValue() * count_period;
    return static_cast<std::chrono::milliseconds>(timeout);
}

void bsp::IndependentWatchDog::Feed()
{
    HAL_IWDG_Refresh(&_handle);
}
