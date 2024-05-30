#include"bsp.h"
#include<Delayer.h>
#include<DigitalLed.h>
#include<functional>
#include<hal.h>
#include<stm32h743-hal-wrapper/Cache.h>
#include<stm32h743-hal-wrapper/clock/ClockSignal.h>
#include<stm32h743-hal-wrapper/clock/Osc.h>

using namespace bsp;

void BSP::Initialize()
{
	auto init_clock = []()
	{
		MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) { }

		hal::OscConfig osc_config;
		osc_config._oscillator_type = hal::OscConfig::OscillatorType::HSE;
		osc_config._hse_state = hal::OscConfig::HseState::On;
		osc_config._hsi_state = hal::OscConfig::HsiState::Off;
		osc_config._csi_state = hal::OscConfig::CsiState::Off;
		osc_config._pll_config._state = hal::PllConfig::PllState::On;
		osc_config._pll_config._source = hal::PllConfig::PllSource::HSE;
		osc_config._pll_config._m = 5;
		osc_config._pll_config._n = 160;
		osc_config._pll_config._p = 2;
		osc_config._pll_config._q = 4;
		osc_config._pll_config._vco_range = hal::PllConfig::Pll1VcoRange::Wide;
		osc_config._pll_config._vci_range = hal::PllConfig::Pll1VciRange::Range2;
		hal::Osc::SetConfig(osc_config);

		hal::ClockSignalConfig clock_signal_config;
		clock_signal_config.SelectAllClockType();
		clock_signal_config._flash_latency = hal::ClockSignalConfig::FlashLatency::Latency2;
		clock_signal_config._system_clk_config._clock_source = hal::SystemClockConfig::ClockSource::PLLCLK;
		clock_signal_config._system_clk_config._output_divider = hal::SystemClockConfig::OutputDivider::DIV1;

		hal::HclkConfig &hclk_config = clock_signal_config._system_clk_config._hclk_config;
		hclk_config._input_divider = hal::HclkConfig::InputDivider::DIV2;
		hclk_config._apb1clk_config._input_divider = hal::Apb1ClkConfig::InputDivider::DIV2;
		hclk_config._apb2clk_config._input_divider = hal::Apb2ClkConfig::InputDivider::DIV2;
		hclk_config._apb3clk_config._input_divider = hal::Apb3ClkConfig::InputDivider::DIV2;
		hclk_config._apb4clk_config._input_divider = hal::Apb4ClkConfig::InputDivider::DIV4;

		hal::ClockSignal::SetConfig(clock_signal_config);

		__HAL_RCC_CSI_ENABLE();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
		HAL_EnableCompensationCell();
	};

	hal::Cache::Enable();
	HAL_Init();
	init_clock();
}

IDigitalLed &BSP::RedDigitalLed()
{
	return RedDigitalLed::Instance();
}

IDigitalLed &BSP::GreenDigitalLed()
{
	return GreenDigitalLed::Instance();
}

bsp::IDelayer &BSP::Delayer()
{
	return Delayer::Instance();
}
