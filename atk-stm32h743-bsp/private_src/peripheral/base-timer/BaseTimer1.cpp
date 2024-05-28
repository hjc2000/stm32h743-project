#include"BaseTimer1.h"
#include<ClockSignal.h>
#include<Interrupt.h>
#include<stdexcept>
#include<task/Critical.h>

using namespace bsp;

extern "C"
{
	void TIM6_IRQHandler()
	{
		HAL_TIM_IRQHandler(&BaseTimer1::Instance()._handle);
	}
}

void bsp::BaseTimer1::Initialize(BaseTimerInitOptions const &options)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	_handle.Instance = TIM6;
	_handle.Init = options;
	HAL_TIM_Base_Init(&_handle);
	_handle.PeriodElapsedCallback = [](TIM_HandleTypeDef *handle)
	{
		try
		{
			if (BaseTimer1::Instance()._on_period_elapsed)
			{
				BaseTimer1::Instance()._on_period_elapsed();
			}
		}
		catch (...)
		{

		}
	};
}

void bsp::BaseTimer1::Initialize(std::chrono::milliseconds period)
{
	uint32_t timer_clock_signal_freq = ClockSignal::Pclk1Freq();
	ClockSignalConfig clock_signal_config = ClockSignal::Config();
	if (clock_signal_config._apb1_divider != ClockSignalConfig::APBDivider::DIV1)
	{
		/*
		* PCLK1 的来源是 HCLK 分频后的输出。如果分频系数大于 1，
		* 则定时器会将 PCLK1 进行 2 倍频后作为自己的时钟信号。
		*/
		timer_clock_signal_freq *= 2;
	}

	/*
	* 将定时器的 16 位分频器和 16 位计数器一起看成一个 32 位的计数器。
	* 这个 32 位计数器的时钟周期是
	*	1 / timer_clock_signal_freq
	* 设计数到 count 刚好达到 period 毫秒。则
	*	count * 1 / timer_clock_signal_freq = period / 1000
	*	count / timer_clock_signal_freq = period / 1000
	*	count = period * timer_clock_signal_freq / 1000
	* 因为 timer_clock_signal_freq 一般较大，所以先进行除法，于是：
	*	count = period * (timer_clock_signal_freq / 1000)
	*/

	uint32_t count = period.count() * (timer_clock_signal_freq / 1000);

	/*
	* 定时器中，分频器相当于这个 32 位计数器的低 16 位，而原本的 16 位计数器相当于
	* 这个 32 位计数器的高 16 位，于是把 count 分成高低 16 位，分别赋予它们。
	*/
	BaseTimerInitOptions options{};
	options._counter_mode = BaseTimerInitOptions::CounterMode::Up;
	options._prescaler = count & 0xffff;
	options._period = count >> 16;
	options._is_auto_reload_preload_enabled = true;
	Initialize(options);
}

void bsp::BaseTimer1::Start()
{
	bsp::Interrupt::SetPriority(TIM6_IRQn, 10, 0);
	bsp::Interrupt::EnableIRQ(TIM6_IRQn);
	HAL_TIM_Base_Start_IT(&_handle);
}

void bsp::BaseTimer1::Stop()
{
	HAL_TIM_Base_Stop_IT(&_handle);
}

void bsp::BaseTimer1::SetPeriodElapsedCallback(std::function<void()> func)
{
	task::Critical::Run([&]()
	{
		_on_period_elapsed = func;
	});
}
