#include "HclkClockSignal.h"
#include <base/unit/Hz.h>

bsp::HclkClockSignal &bsp::HclkClockSignal::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<HclkClockSignal>
    {
    public:
        std::unique_ptr<HclkClockSignal> Create() override
        {
            return std::unique_ptr<HclkClockSignal>{new HclkClockSignal{}};
        }
    };

    Getter g;
    return g.Instance();
}

std::string bsp::HclkClockSignal::Name() const
{
    return "hclk";
}

base::MHz bsp::HclkClockSignal::Frequency() const
{
    uint32_t value = HAL_RCC_GetHCLKFreq();
    return base::MHz{base::Hz{value}};
}

void bsp::HclkClockSignal::Open(bsp::IClockSignal_InputDivisionFactor const &input_division_factor)
{
    RCC_ClkInitTypeDef def{};
    def.ClockType = RCC_CLOCKTYPE_HCLK;
    switch (input_division_factor.Value())
    {
    case 1:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV1;
            break;
        }
    case 2:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV2;
            break;
        }
    case 4:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV4;
            break;
        }
    case 8:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV8;
            break;
        }
    case 16:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV16;
            break;
        }
    case 64:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV64;
            break;
        }
    case 128:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV128;
            break;
        }
    case 256:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV256;
            break;
        }
    case 512:
        {
            def.AHBCLKDivider = RCC_HCLK_DIV512;
            break;
        }
    default:
        {
            throw std::invalid_argument{"不支持的分频系数"};
        }
    }

    HAL_StatusTypeDef ret = HAL_RCC_ClockConfig(&def,
                                                FLASH_LATENCY_2);

    if (ret != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"时钟信号配置失败"};
    }
}
