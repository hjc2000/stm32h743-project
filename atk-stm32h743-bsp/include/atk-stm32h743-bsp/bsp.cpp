#include"bsp.h"
#include<Cache.h>
#include<Delayer.h>
#include<DigitalLed.h>
#include<functional>
#include<hal.h>
#include<Osc.h>
#include<stm32h743-hal-wrapper/ClockSignal.h>

using namespace bsp;

void BSP::Initialize()
{
	auto init_clock = []()
	{
		MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) { }

		OscConfig osc_config;
		osc_config._oscillator_type = OscConfig::OscillatorType::HSE;
		osc_config._hse_state = OscConfig::HseState::On;
		osc_config._hsi_state = OscConfig::HsiState::Off;
		osc_config._csi_state = OscConfig::CsiState::Off;
		osc_config._pll_config._state = PllConfig::PllState::On;
		osc_config._pll_config._source = PllConfig::PllSource::HSE;
		osc_config._pll_config._m = 5;
		osc_config._pll_config._n = 160;
		osc_config._pll_config._p = 2;
		osc_config._pll_config._q = 4;
		osc_config._pll_config._vco_range = PllConfig::Pll1VcoRange::Wide;
		osc_config._pll_config._vci_range = PllConfig::Pll1VciRange::Range2;
		Osc::SetConfig(osc_config);

		ClockSignalConfig clock_signal_config;
		clock_signal_config.SelectAllClockType();
		clock_signal_config._flash_latency = ClockSignalConfig::FlashLatency::Latency2;
		clock_signal_config._system_clk_config._clock_source = SystemClockConfig::ClockSource::PLLCLK;
		clock_signal_config._system_clk_config._output_divider = SystemClockConfig::OutputDivider::DIV1;

		HclkConfig &hclk_config = clock_signal_config._system_clk_config._hclk_config;
		hclk_config._input_divider = HclkConfig::InputDivider::DIV2;
		hclk_config._apb1clk_config._input_divider = Apb1ClkConfig::InputDivider::DIV2;
		hclk_config._apb2clk_config._input_divider = Apb2ClkConfig::InputDivider::DIV2;
		hclk_config._apb3clk_config._input_divider = Apb3ClkConfig::InputDivider::DIV2;
		hclk_config._apb4clk_config._input_divider = Apb4ClkConfig::InputDivider::DIV4;

		ClockSignal::SetConfig(clock_signal_config);

		__HAL_RCC_CSI_ENABLE();
		__HAL_RCC_SYSCFG_CLK_ENABLE();
		HAL_EnableCompensationCell();
	};

	Cache::Enable();
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
