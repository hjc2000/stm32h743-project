#include "SysclkClockSignal.h"
#include <base/unit/Hz.h>

bsp::SysclkClockSignal &bsp::SysclkClockSignal::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<SysclkClockSignal>
    {
    public:
        std::unique_ptr<SysclkClockSignal> Create() override
        {
            return std::unique_ptr<SysclkClockSignal>{new SysclkClockSignal{}};
        }
    };

    Getter g;
    return g.Instance();
}

std::string bsp::SysclkClockSignal::Name() const
{
    return "sysclk";
}

base::MHz bsp::SysclkClockSignal::Frequency() const
{
    uint32_t value = HAL_RCC_GetSysClockFreq();
    return base::MHz{base::Hz{value}};
}

void bsp::SysclkClockSignal::Open(bsp::IClockSignal_OutputDivisionFactor const &output_division_factor,
                                  IClockSignal_ClockSource const &clock_source)
{
    RCC_ClkInitTypeDef def{};
    def.ClockType = RCC_CLOCKTYPE_SYSCLK;

    if (clock_source.Value() == "hse")
    {
        def.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
    }
    else if (clock_source.Value() == "hsi")
    {
        def.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    }
    else if (clock_source.Value() == "csi")
    {
        def.SYSCLKSource = RCC_SYSCLKSOURCE_CSI;
    }
    else if (clock_source.Value() == "pll")
    {
        def.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    }
    else
    {
        throw std::invalid_argument{"不支持此时钟源"};
    }

    switch (output_division_factor.Value())
    {
    case 1:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV1;
            break;
        }
    case 2:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV2;
            break;
        }
    case 4:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV4;
            break;
        }
    case 8:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV8;
            break;
        }
    case 16:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV16;
            break;
        }
    case 64:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV64;
            break;
        }
    case 128:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV128;
            break;
        }
    case 256:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV256;
            break;
        }
    case 512:
        {
            def.SYSCLKDivider = RCC_SYSCLK_DIV512;
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
