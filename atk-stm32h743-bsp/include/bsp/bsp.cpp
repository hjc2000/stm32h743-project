#include "bsp.h"
#include <DigitalLed.h>
#include <Key.h>
#include <atomic>
#include <base/Initializer.h>
#include <base/container/StdMapValuesEnumerable.h>
#include <bsp-interface/di.h>
#include <bsp-interface/key/KeyScanner.h>
#include <functional>
#include <hal-wrapper/Cache.h>
#include <hal-wrapper/Flash.h>
#include <hal-wrapper/clock/ClockSignal.h>
#include <hal-wrapper/clock/Osc.h>
#include <bsp-interface/di.h>
#include <hal-wrapper/peripheral/independent-watch-dog/IndependentWatchDog.h>
#include <hal-wrapper/peripheral/serial/Serial.h>
#include <hal-wrapper/peripheral/timer/PwmModeTimer3.h>
#include <hal.h>
#include <stdint.h>
#include <task/TaskDelayer.h>

using namespace bsp;

static base::Initializer _key_scanner_initializer{
	[]()
	{
		DI_KeyCollection();
		DI_KeyScanner();
	}};

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
	base::Initializer::Initialize();
}

IDigitalLed &BSP::RedDigitalLed()
{
	return RedDigitalLed::Instance();
}

IDigitalLed &BSP::GreenDigitalLed()
{
	return GreenDigitalLed::Instance();
}

bsp::ISerial &BSP::Serial()
{
	return hal::Serial::Instance();
}

bsp::IIndependentWatchDog &BSP::IndependentWatchDog()
{
	return hal::IndependentWatchDog::Instance();
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
		BSP::GreenDigitalLed().Toggle();

		value += config.Period() + config.Period() / 4;
		value %= config.Period();
		compare_output_config.SetPulse(value);
		hal::PwmModeTimer3::Instance().ConfigPwmChannel(compare_output_config, hal::TimerChannelEnum::Channel4);
	}
}

volatile bool _should_toggle = false;
uint64_t _count = 0;

void TestFlash()
{
	try
	{
		auto &flash = hal::Flash::Instance();
		std::array<uint32_t, 8> buffer = {666, 2, 3};
		while (true)
		{
			DI_KeyScanner().ScanKeys();
			if (DI_KeyScanner().HasKeyDownEvent("key0"))
			{
				base::UnlockGuard ul{flash};
				_should_toggle = true;
				// flash.EraseBank(2);
				flash.EraseSector(1, 0);
				uint32_t value = flash.ReadUInt32(1, 0);
				flash.Program(1, 0, reinterpret_cast<uint8_t *>(buffer.data()));
				value = flash.ReadUInt32(1, 0);
				_should_toggle = false;
				BSP::GreenDigitalLed().Toggle();
			}
		}
	}
	catch (std::exception const &e)
	{
		std::string str = e.what();
	}
}

void TestGpio()
{
	hal::GpioPinConfig options;
	options.SetPin(hal::GpioPinConfig::PinEnum::Pin4);
	options.SetMode(hal::GpioPinConfig::ModeEnum::Output_PushPull);
	options.SetPull(hal::GpioPinConfig::PullOption::PullUp);
	options.SetSpeed(hal::GpioPinConfig::SpeedOption::High);
	hal::GpioPortA::Instance().InitPin(options);

	while (true)
	{
		// bsp::DI_Delayer().Delay(std::chrono::milliseconds{1});
		_count++;
		if (_should_toggle)
		{
			hal::GpioPortA::Instance().DigitalTogglePin(hal::GpioPinConfig::PinEnum::Pin4);
		}
		else
		{
			hal::GpioPortA::Instance().DigitalWritePin(hal::GpioPinConfig::PinEnum::Pin4, 0);
		}
	}
}

#pragma region DI_KeyScanner
/// @brief 按键集合。
/// @return
base::IReadOnlyCollection<std::string, bsp::IKey *> &DI_KeyCollection()
{
	class Collection
		: public base::IReadOnlyCollection<std::string, bsp::IKey *>
	{
	private:
		std::map<std::string, bsp::IKey *> _map;

		void AddKey(bsp::IKey *key)
		{
			_map[key->KeyName()] = key;
		}

	public:
		Collection()
		{
			AddKey(&Key0::Instance());
			AddKey(&Key1::Instance());
			AddKey(&Key2::Instance());
		}

		int Count() const override
		{
			return static_cast<int>(_map.size());
		}

		bsp::IKey *Get(std::string key) const override
		{
			auto map = const_cast<Collection *>(this)->_map;
			auto it = map.find(key);
			if (it == map.end())
			{
				throw std::runtime_error{"找不到此按键"};
			}

			return it->second;
		}

		std::shared_ptr<base::IEnumerator<bsp::IKey *>> GetEnumerator() override
		{
			return std::shared_ptr<base::IEnumerator<bsp::IKey *>>{
				new base::StdMapValuesEnumerator<std::string, bsp::IKey *>{&_map},
			};
		}
	};

	static Collection o;
	return o;
}

bsp::IKeyScanner &DI_KeyScanner()
{
	static bsp::KeyScanner o{};
	return o;
}
#pragma endregion
