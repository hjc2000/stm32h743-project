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

        hal::ClockSignalConfig clock_signal_config;
        clock_signal_config.SelectAllClockType();
        clock_signal_config._flash_latency = hal::ClockSignalConfig::FlashLatency::Latency2;
        clock_signal_config._system_clk_config._clock_source = hal::SystemClockConfig::ClockSource::PLLCLK;
        clock_signal_config._system_clk_config._output_divider = hal::SystemClockConfig::OutputDivider::DIV1;
        clock_signal_config._system_clk_config._hclk_config._input_divider = hal::HclkConfig::InputDivider::DIV2;
        clock_signal_config._system_clk_config._hclk_config._apb1clk_config._input_divider = hal::Apb1ClkConfig::InputDivider::DIV2;
        clock_signal_config._system_clk_config._hclk_config._apb2clk_config._input_divider = hal::Apb2ClkConfig::InputDivider::DIV2;
        clock_signal_config._system_clk_config._hclk_config._apb3clk_config._input_divider = hal::Apb3ClkConfig::InputDivider::DIV2;
        clock_signal_config._system_clk_config._hclk_config._apb4clk_config._input_divider = hal::Apb4ClkConfig::InputDivider::DIV4;
        hal::ClockSignal::SetConfig(clock_signal_config);

        __HAL_RCC_CSI_ENABLE();
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
