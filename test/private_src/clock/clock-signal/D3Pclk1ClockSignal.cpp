#include "D3Pclk1ClockSignal.h"
#include <base/unit/Hz.h>

bsp::D3Pclk1ClockSignal &bsp::D3Pclk1ClockSignal::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<D3Pclk1ClockSignal>
    {
    public:
        std::unique_ptr<D3Pclk1ClockSignal> Create() override
        {
            return std::unique_ptr<D3Pclk1ClockSignal>{new D3Pclk1ClockSignal{}};
        }
    };

    Getter g;
    return g.Instance();
}

std::string bsp::D3Pclk1ClockSignal::Name() const
{
    return "d3pclk1";
}

base::MHz bsp::D3Pclk1ClockSignal::Frequency() const
{
    uint32_t value = HAL_RCC_GetHCLKFreq() / _division_factor;
    return base::MHz{base::Hz{value}};
}

void bsp::D3Pclk1ClockSignal::Open(bsp::IClockSignal_InputDivisionFactor const &input_division_factor)
{
    RCC_ClkInitTypeDef def{};
    def.ClockType = RCC_CLOCKTYPE_D3PCLK1;

    switch (input_division_factor.Value())
    {
    case 1:
        {
            def.APB4CLKDivider = RCC_APB4_DIV1;
            break;
        }
    case 2:
        {
            def.APB4CLKDivider = RCC_APB4_DIV2;
            break;
        }
    case 4:
        {
            def.APB4CLKDivider = RCC_APB4_DIV4;
            break;
        }
    case 8:
        {
            def.APB4CLKDivider = RCC_APB4_DIV8;
            break;
        }
    case 16:
        {
            def.APB4CLKDivider = RCC_APB4_DIV16;
            break;
        }
    default:
        {
            throw std::invalid_argument{"不支持此分频"};
        }
    }

    // 通过了上面的 switch 语句不抛出异常后才能更改 _division_factor
    _division_factor = input_division_factor.Value();

    HAL_StatusTypeDef ret = HAL_RCC_ClockConfig(&def,
                                                FLASH_LATENCY_2);

    if (ret != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"时钟信号配置失败"};
    }
}
