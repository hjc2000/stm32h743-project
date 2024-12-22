#include <base/container/Dictionary.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/reset_initialize.h>
#include <hal.h>

namespace
{
	bool _initialized = false;

	void InitializeClock()
	{
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
	}
} // namespace

void DI_Initialize()
{
	if (_initialized)
	{
		return;
	}

	_initialized = true;

	DI_InitializeCPU();
	InitializeClock();
}
