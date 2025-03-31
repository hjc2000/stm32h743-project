#include "Pclk2ClockSignal.h"
#include <base/unit/Hz.h>

bsp::Pclk2ClockSignal &bsp::Pclk2ClockSignal::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<Pclk2ClockSignal>
    {
    public:
        std::unique_ptr<Pclk2ClockSignal> Create() override
        {
            return std::unique_ptr<Pclk2ClockSignal>{new Pclk2ClockSignal{}};
        }
    };

    Getter g;
    return g.Instance();
}

std::string bsp::Pclk2ClockSignal::Name() const
{
    return "pclk2";
}

base::MHz bsp::Pclk2ClockSignal::Frequency() const
{
    uint32_t value = HAL_RCC_GetPCLK2Freq();
    return base::MHz{base::Hz{value}};
}

void bsp::Pclk2ClockSignal::Open(bsp::IClockSignal_InputDivisionFactor const &input_division_factor)
{
    RCC_ClkInitTypeDef def{};
    def.ClockType = RCC_CLOCKTYPE_PCLK2;

    switch (input_division_factor.Value())
    {
    case 1:
        {
            def.APB2CLKDivider = RCC_APB2_DIV1;
            break;
        }
    case 2:
        {
            def.APB2CLKDivider = RCC_APB2_DIV2;
            break;
        }
    case 4:
        {
            def.APB2CLKDivider = RCC_APB2_DIV4;
            break;
        }
    case 8:
        {
            def.APB2CLKDivider = RCC_APB2_DIV8;
            break;
        }
    case 16:
        {
            def.APB2CLKDivider = RCC_APB2_DIV16;
            break;
        }
    default:
        {
            throw std::invalid_argument{"不支持此分频"};
        }
    }

    HAL_StatusTypeDef ret = HAL_RCC_ClockConfig(&def,
                                                FLASH_LATENCY_2);

    if (ret != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"时钟信号配置失败"};
    }
}
