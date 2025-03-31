#include "PllClockSource.h"
#include <bsp-interface/di/clock.h>

bsp::PllClockSource &bsp::PllClockSource::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<PllClockSource>
    {
    public:
        std::unique_ptr<PllClockSource> Create() override
        {
            return std::unique_ptr<PllClockSource>{new PllClockSource{}};
        }
    };

    Getter g;
    return g.Instance();
}

std::string bsp::PllClockSource::Name() const
{
    return "pll";
}

void bsp::PllClockSource::Open(std::string const &input_channel_name,
                               base::IDictionary<std::string, int> const &factors)
{
#pragma region m,n,p,q,r
    int m = 1;

    {
        int const *ptr = factors.Find("m");
        if (ptr != nullptr)
        {
            m = *ptr;
        }
    }

    int n = 1;

    {
        int const *ptr = factors.Find("n");
        if (ptr != nullptr)
        {
            n = *ptr;
        }
    }

    int p = 1;

    {
        int const *ptr = factors.Find("p");
        if (ptr != nullptr)
        {
            p = *ptr;
        }
    }

    int q = 1;

    {
        int const *ptr = factors.Find("q");
        if (ptr != nullptr)
        {
            q = *ptr;
        }
    }

    int r = 1;

    {
        int const *ptr = factors.Find("r");
        if (ptr != nullptr)
        {
            r = *ptr;
        }
    }
#pragma endregion

#pragma region pll_source
    uint32_t pll_source = RCC_PLLSOURCE_HSE;
    if (input_channel_name == "hse")
    {
        pll_source = RCC_PLLSOURCE_HSE;
    }
    else if (input_channel_name == "hsi")
    {
        pll_source = RCC_PLLSOURCE_HSI;
    }
    else if (input_channel_name == "csi")
    {
        pll_source = RCC_PLLSOURCE_CSI;
    }
    else
    {
        throw std::invalid_argument{"不支持该输入通道"};
    }
#pragma endregion

#pragma region pll_range
    base::MHz input_freq;
    int pll_range = RCC_PLL1VCIRANGE_2;
    if (input_channel_name == "hse")
    {
        input_freq = bsp::di::clock::ClockSourceCollection().Get("hse")->Frequency();
        base::Fraction fraction_input_freq = static_cast<base::Fraction>(input_freq);

        /* 经过 m 分频系数分频后输入锁相环，这里需要根据输入锁相环的频率所处的范围来设置参数。
         */
        fraction_input_freq = fraction_input_freq / m;
        if (fraction_input_freq < 2)
        {
            pll_range = RCC_PLL1VCIRANGE_0;
        }
        else if (fraction_input_freq >= 2 && fraction_input_freq < 4)
        {
            pll_range = RCC_PLL1VCIRANGE_1;
        }
        else if (fraction_input_freq >= 4 && fraction_input_freq < 8)
        {
            pll_range = RCC_PLL1VCIRANGE_2;
        }
        else
        {
            pll_range = RCC_PLL1VCIRANGE_3;
        }
    }
    else if (input_channel_name == "hsi")
    {
        throw std::invalid_argument{"不支持该输入通道"};
    }
    else if (input_channel_name == "csi")
    {
        throw std::invalid_argument{"不支持该输入通道"};
    }
    else
    {
        throw std::invalid_argument{"不支持该输入通道"};
    }
#pragma endregion

    RCC_OscInitTypeDef def{};
    def.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    def.PLL.PLLState = RCC_PLL_ON;
    def.PLL.PLLSource = pll_source;
    def.PLL.PLLRGE = pll_range;
    def.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    def.PLL.PLLM = m;
    def.PLL.PLLN = n;
    def.PLL.PLLP = p;
    def.PLL.PLLQ = q;
    def.PLL.PLLR = r;

    HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
    if (result != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"打开 PLL 失败。"};
    }

    // 打开后，记录各个输出通道的频率
    _p_freq = input_freq / m * n / p;
    _q_freq = input_freq / m * n / q;
    _r_freq = input_freq / m * n / r;

    _opened = true;
}

void bsp::PllClockSource::Close()
{
    RCC_OscInitTypeDef def{};
    def.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    def.PLL.PLLState = RCC_PLL_OFF;
    HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
    if (result != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"关闭 PLL 失败。"};
    }

    _opened = false;
}

bsp::IClockSource_State bsp::PllClockSource::State() const
{
    return _state;
}

base::MHz bsp::PllClockSource::Frequency(std::string const &output_channel_name) const
{
    if (!_opened)
    {
        throw std::runtime_error{"pll 还未打开，无法查看频率"};
    }

    if (output_channel_name == "p")
    {
        return _p_freq;
    }
    else if (output_channel_name == "q")
    {
        return _q_freq;
    }
    else if (output_channel_name == "r")
    {
        return _r_freq;
    }
    else
    {
        throw std::invalid_argument{"没有该输出通道"};
    }
}
