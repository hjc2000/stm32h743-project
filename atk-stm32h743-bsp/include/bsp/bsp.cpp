#include "bsp.h"
#include <DigitalLed.h>
#include <ExtiWakeUpKey.h>
#include <Key.h>
#include <atomic>
#include <bsp-interface/key/KeyScanner.h>
#include <functional>
#include <hal-wrapper/Cache.h>
#include <hal-wrapper/Flash.h>
#include <hal-wrapper/clock/ClockSignal.h>
#include <hal-wrapper/clock/Osc.h>
#include <hal-wrapper/interrupt/InterruptSwitch.h>
#include <hal-wrapper/peripheral/independent-watch-dog/IndependentWatchDog.h>
#include <hal-wrapper/peripheral/serial/Serial.h>
#include <hal-wrapper/peripheral/timer/PwmModeTimer3.h>
#include <hal-wrapper/peripheral/window-watch-dog/WindowWatchDog.h>
#include <hal.h>
#include <stdint.h>
#include <task/TaskDelayer.h>

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

		hal::OscConfig osc_config;
		osc_config._oscillator_type = hal::OscConfig::OscillatorType::HSE;
		osc_config._hse_state = hal::OscConfig::HseState::On;
		osc_config._hsi_state = hal::OscConfig::HsiState::Off;
		osc_config._csi_state = hal::OscConfig::CsiState::Off;
		osc_config._pll_config._state = hal::PllConfig::PllState::On;
		osc_config._pll_config._source = hal::PllConfig::PllSource::HSE;
		osc_config._pll_config._m = 5;
		osc_config._pll_config._n = 192;
		osc_config._pll_config._p = 2;
		osc_config._pll_config._q = 2;
		osc_config._pll_config._r = 2;
		osc_config._pll_config._vco_range = hal::PllConfig::Pll1VcoRange::Wide;
		osc_config._pll_config._vci_range = hal::PllConfig::Pll1VciRange::Range2;
		hal::Osc::SetConfig(osc_config);

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

IDigitalLed &BSP::RedDigitalLed()
{
	return RedDigitalLed::Instance();
}

IDigitalLed &BSP::GreenDigitalLed()
{
	return GreenDigitalLed::Instance();
}

bsp::IKeyScanner &BSP::KeyScanner()
{
	class KeyScannerInitializer
	{
	private:
		KeyScannerInitializer()
		{
			_keys[(uint16_t)KeyIndex::Key0] = &Key0::Instance();
			_keys[(uint16_t)KeyIndex::Key1] = &Key1::Instance();
			_keys[(uint16_t)KeyIndex::Key2] = &Key2::Instance();
			_keys[(uint16_t)KeyIndex::KeyWakeUp] = &KeyWakeUp::Instance();
			_key_scanner = std::shared_ptr<bsp::KeyScanner>{new bsp::KeyScanner{_keys}};
		}

		KeyScannerInitializer(KeyScannerInitializer const &o) = delete;
		KeyScannerInitializer(KeyScannerInitializer &&o) = delete;
		KeyScannerInitializer &operator=(KeyScannerInitializer const &o) = delete;
		KeyScannerInitializer &operator=(KeyScannerInitializer &&o) = delete;

		std::vector<bsp::IKey *> _keys{(size_t)KeyIndex::EnumEndFlag};
		std::shared_ptr<bsp::KeyScanner> _key_scanner;

	public:
		static KeyScannerInitializer &Instance()
		{
			static KeyScannerInitializer o;
			return o;
		}

		IKeyScanner &Scanner()
		{
			return *_key_scanner;
		}
	};

	return KeyScannerInitializer::Instance().Scanner();
}

bsp::IEventDrivenKey &BSP::WakeUpKey()
{
	return bsp::ExtiWakeUpKey::Instance();
}

bsp::ISerial &BSP::Serial()
{
	return hal::Serial::Instance();
}

bsp::IIndependentWatchDog &BSP::IndependentWatchDog()
{
	return hal::IndependentWatchDog::Instance();
}

bsp::IInterruptSwitch &BSP::InterruptSwitch()
{
	return hal::InterruptSwitch::Instance();
}

void TestWindowWatchDog()
{
	bsp::DI_Delayer().Delay(std::chrono::seconds{1});
	BSP::RedDigitalLed().TurnOn();

	hal::WindowWatchDogConfig config;
	config.SetCounterReloadValue(0x7f);
	config.SetWindow(0x5f);
	config.SetPrescaler(hal::WindowWatchDogConfig::PrescalerOption::DIV8);
	config.SetEarlyWakeupInterrupt(hal::WindowWatchDogConfig::EarlyWakeupInterruptOption::Enable);

	hal::WindowWatchDog::Instance().SetEarlyWakeupInterruptCallback([&]()
																	{ BSP::GreenDigitalLed().Toggle(); });

	hal::WindowWatchDog::Instance().Initialize(config);

	while (true)
	{
		BSP::RedDigitalLed().Toggle();
		bsp::DI_Delayer().Delay(std::chrono::seconds{1});
	}
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
		bsp::DI_Delayer().Delay(std::chrono::milliseconds{1000});
		BSP::GreenDigitalLed().Toggle();

		value += config.Period() + config.Period() / 4;
		value %= config.Period();
		compare_output_config.SetPulse(value);
		hal::PwmModeTimer3::Instance().ConfigPwmChannel(compare_output_config, hal::TimerChannelEnum::Channel4);
	}
}

void TestFlash()
{
	try
	{
		auto &flash = hal::Flash::Instance();
		while (true)
		{
			BSP::KeyScanner().ScanKeys();
			if (BSP::KeyScanner().HasKeyDownEvent(static_cast<uint16_t>(KeyIndex::Key0)))
			{
				base::UnlockGuard ul{flash};
				uint32_t value = flash.ReadUInt32(2, 10 * 128);
				// flash.EraseBank(2);
				for (int i = 0; i <= 7; i++)
				{
					flash.EraseSector(2, i, 1);
				}

				value = flash.ReadUInt32(2, 10 * 128);
				BSP::GreenDigitalLed().Toggle();
			}
		}
	}
	catch (std::exception const &e)
	{
		std::string str = e.what();
	}
}
