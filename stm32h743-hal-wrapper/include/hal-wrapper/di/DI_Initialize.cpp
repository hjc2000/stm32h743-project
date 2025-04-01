#include <base/container/Dictionary.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/reset_initialize.h>
#include <hal.h>

namespace
{
	bool _initialized = false;

	void InitializeClock()
	{
		bsp::di::clock::ClockSourceCollection().Get("hse")->Open(base::MHz{25});

		{
			base::Dictionary<std::string, int> factors;

			/**
			 * HSE 接的是 25MHz 的晶振。
			 * 		@li m 是对 PLL 输入时钟的分频。25MHz 的晶振分频后是 5MHz.
			 * 		@li n 是对经过 m 分频后的信号进行倍频。5MHz 被 192 倍频后是 960MHz.
			 * 		@li p,q,r 是对 n 的输出进行分频。这里是 2 分频，于是得到的都是 480MHz 的时钟信号。
			 * 			其中 p 的输出作为系统时钟的输入。于是系统时钟频率是 480MHz. 系统时钟直接输入给 CPU.
			 */
			factors.Set("m", 5);
			factors.Set("n", 192);
			factors.Set("p", 2);
			factors.Set("q", 2);
			factors.Set("r", 2);
			bsp::di::clock::ClockSourceCollection().Get("pll")->Open("hse", factors);
		}

		bsp::di::clock::ClockSignalCollection().Get("hclk")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("pclk2")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("d1pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("d3pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});

		bsp::di::clock::ClockSignalCollection().Get("sysclk")->Open(bsp::IClockSignal_OutputDivisionFactor{1},
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
