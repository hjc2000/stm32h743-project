#include "Pclk1ClockSignal.h"
#include <base/unit/Hz.h>

bsp::Pclk1ClockSignal &bsp::Pclk1ClockSignal::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<Pclk1ClockSignal>
    {
    public:
        std::unique_ptr<Pclk1ClockSignal> Create() override
        {
            return std::unique_ptr<Pclk1ClockSignal>{new Pclk1ClockSignal{}};
        }
    };

    Getter g;
    return g.Instance();
}

std::string bsp::Pclk1ClockSignal::Name() const
{
    return "pclk1";
}

base::MHz bsp::Pclk1ClockSignal::Frequency() const
{
    uint32_t value = HAL_RCC_GetPCLK1Freq();
    return base::MHz{base::Hz{value}};
}

void bsp::Pclk1ClockSignal::Open(bsp::IClockSignal_InputDivisionFactor const &input_division_factor)
{
    RCC_ClkInitTypeDef def{};
    def.ClockType = RCC_CLOCKTYPE_PCLK1;

    switch (input_division_factor.Value())
    {
    case 1:
        {
            def.APB1CLKDivider = RCC_APB1_DIV1;
            break;
        }
    case 2:
        {
            def.APB1CLKDivider = RCC_APB1_DIV2;
            break;
        }
    case 4:
        {
            def.APB1CLKDivider = RCC_APB1_DIV4;
            break;
        }
    case 8:
        {
            def.APB1CLKDivider = RCC_APB1_DIV8;
            break;
        }
    case 16:
        {
            def.APB1CLKDivider = RCC_APB1_DIV16;
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
