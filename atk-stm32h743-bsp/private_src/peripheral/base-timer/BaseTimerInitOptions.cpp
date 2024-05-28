#include"BaseTimerInitOptions.h"

using namespace bsp;

bsp::BaseTimerInitOptions::BaseTimerInitOptions(TIM_Base_InitTypeDef const &o)
{
	*this = o;
}

BaseTimerInitOptions &bsp::BaseTimerInitOptions::operator=(TIM_Base_InitTypeDef const &o)
{
	_prescaler = o.Prescaler;
	_counter_mode = static_cast<CounterMode>(o.CounterMode);
	_period = o.Period;
	_is_auto_reload_preload_enabled = TIM_AUTORELOAD_PRELOAD_ENABLE == o.AutoReloadPreload;
	return *this;
}

bsp::BaseTimerInitOptions::operator TIM_Base_InitTypeDef() const
{
	TIM_Base_InitTypeDef o;
	o.Prescaler = _prescaler;
	o.CounterMode = static_cast<uint32_t>(_counter_mode);
	o.Period = _period;
	if (_is_auto_reload_preload_enabled)
	{
		o.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	}
	else
	{
		o.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	}

	return o;
}
