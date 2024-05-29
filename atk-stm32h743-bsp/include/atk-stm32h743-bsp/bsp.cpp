#include"bsp.h"
#include<Cache.h>
#include<ClockSignal.h>
#include<Delayer.h>
#include<DigitalLed.h>
#include<functional>
#include<hal.h>

using namespace bsp;

void BSP::Initialize()
{
	auto init_clock = []()
	{
		HAL_StatusTypeDef ret = HAL_OK;

		MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) { }

		RCC_OscInitTypeDef RCC_OscInitStruct;
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
		RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

		RCC_OscInitStruct.PLL.PLLN = 160;
		RCC_OscInitStruct.PLL.PLLM = 5;
		RCC_OscInitStruct.PLL.PLLP = 2;
		RCC_OscInitStruct.PLL.PLLQ = 4;

		RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
		RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
		ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
		if (ret != HAL_OK) while (1);

		ClockSignalConfig clock_signal_config;
		clock_signal_config.SelectAllClockType();
		clock_signal_config._flash_latency = ClockSignalConfig::FlashLatency::Latency2;
		clock_signal_config._system_clk_config._clock_source = SystemClockConfig::ClockSource::PLLCLK;
		clock_signal_config._system_clk_config._output_divider = SystemClockConfig::OutputDivider::DIV1;
		clock_signal_config._system_clk_config._hclk_config._input_divider = HclkConfig::InputDivider::DIV2;
		clock_signal_config._system_clk_config._hclk_config._apb1clk_config._input_divider = Apb1ClkConfig::InputDivider::DIV2;
		clock_signal_config._system_clk_config._hclk_config._apb2clk_config._input_divider = Apb2ClkConfig::InputDivider::DIV2;
		clock_signal_config._system_clk_config._hclk_config._apb3clk_config._input_divider = Apb3ClkConfig::InputDivider::DIV2;
		clock_signal_config._system_clk_config._hclk_config._apb4clk_config._input_divider = Apb4ClkConfig::InputDivider::DIV4;
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
