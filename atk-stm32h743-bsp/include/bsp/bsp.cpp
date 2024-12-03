#include "bsp.h"
#include <atomic>
#include <base/container/Dictionary.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/flash.h>
#include <bsp-interface/di/key.h>
#include <bsp-interface/di/led.h>
#include <DigitalLed.h>
#include <functional>
#include <hal-wrapper/Cache.h>
#include <hal-wrapper/clock/ClockSignal.h>
#include <hal-wrapper/peripheral/timer/PwmModeTimer3.h>
#include <hal.h>
#include <stdint.h>

using namespace bsp;

void BSP::Initialize()
{
    auto init_clock = []()
    {
        MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY)
        {
        }

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

        // DI_ClockSignalCollection().Get("sysclk")->Open(bsp::IClockSignal_OutputDivisionFactor{1},
        //                                                bsp::IClockSignal_ClockSource{"pll"});

        /* 时钟源的特点是可能有多个输入、输出通道。
         *
         * 时钟信号的特点是输入通道只有一个，输出通道一般有多个，因为要分给各个子系统使用。
         * 并且支持灵活的输入、输出分频。
         *
         * 系统时钟比较特殊，输入通道有多个，可以选择不同的时钟源。
         */
        hal::ClockSignalConfig clock_signal_config;
        clock_signal_config.SelectAllClockType();
        clock_signal_config._flash_latency = hal::ClockSignalConfig::FlashLatency::Latency2;
        clock_signal_config._system_clk_config._clock_source = hal::SystemClockConfig::ClockSource::PLLCLK;
        clock_signal_config._system_clk_config._output_divider = hal::SystemClockConfig::OutputDivider::DIV1;
        clock_signal_config._system_clk_config._hclk_config._apb4clk_config._input_divider = hal::Apb4ClkConfig::InputDivider::DIV4;
        hal::ClockSignal::SetConfig(clock_signal_config);

        __HAL_RCC_SYSCFG_CLK_ENABLE();
        HAL_EnableCompensationCell();
    };

    hal::Cache::Enable();
    HAL_Init();
    init_clock();
}

void TestUniversalTimer1()
{
    hal::UniversalTimerBaseConfig config;
    config.SetPrescaler(200 - 1);
    config.SetPeriod(500 - 1);
    config.SetCounterMode(hal::UniversalTimerBaseConfig::CounterModeOption::UP);
    config.SetClockDivision(hal::UniversalTimerBaseConfig::ClockDivisionOption::DIV1);
    hal::PwmModeTimer3::Instance().PwmInitialize(config);
    hal::UniversalTimerCompareOutputConfig compare_output_config;
    compare_output_config.SetMode(hal::UniversalTimerCompareOutputConfig::ModeOption::Pwm1);
    compare_output_config.SetPulse(500 / 2);
    compare_output_config.SetPolarity(hal::UniversalTimerCompareOutputConfig::PolarityOption::Low);
    hal::PwmModeTimer3::Instance().ConfigPwmChannel(compare_output_config, hal::TimerChannelEnum::Channel4);
    hal::PwmModeTimer3::Instance().StartPwm(hal::TimerChannelEnum::Channel4);

    uint32_t value = 500 / 2;
    while (true)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{1000});
        value += config.Period() + config.Period() / 4;
        value %= config.Period();
        compare_output_config.SetPulse(value);
        hal::PwmModeTimer3::Instance().ConfigPwmChannel(compare_output_config, hal::TimerChannelEnum::Channel4);
    }
}
