#include <base/container/Dictionary.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/core.h>
#include <hal-wrapper/Cache.h>
#include <hal.h>

void DI_DoBasicInitialization()
{
    auto init_clock = []()
    {
        MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY)
        {
        }

        __HAL_RCC_SYSCFG_CLK_ENABLE();
        HAL_EnableCompensationCell();

        DI_ClockSourceCollection().Get("hse")->Open(25);

        {
            base::Dictionary<std::string, int> factors;
            factors.Set("m", 5);
            factors.Set("n", 192);
            factors.Set("p", 2);
            factors.Set("q", 2);
            factors.Set("r", 2);
            DI_ClockSourceCollection().Get("pll")->Open("hse", factors);
        }

        DI_ClockSignalCollection().Get("hclk")->Open(bsp::IClockSignal_InputDivisionFactor{2});
        DI_ClockSignalCollection().Get("pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});
        DI_ClockSignalCollection().Get("pclk2")->Open(bsp::IClockSignal_InputDivisionFactor{2});
        DI_ClockSignalCollection().Get("d1pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});
        DI_ClockSignalCollection().Get("d3pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});

        DI_ClockSignalCollection().Get("sysclk")->Open(bsp::IClockSignal_OutputDivisionFactor{1},
                                                       bsp::IClockSignal_ClockSource{"pll"});
    };

    hal::Cache::Enable();
    HAL_Init();
    init_clock();
}
