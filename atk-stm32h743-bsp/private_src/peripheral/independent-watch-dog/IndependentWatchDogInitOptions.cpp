#include"IndependentWatchDogInitOptions.h"

using namespace bsp;

IndependentWatchDogPrescaler bsp::PowerToIndependentWatchDogPrescaler(uint8_t pow)
{
	switch (pow)
	{
	case 2:
		{
			return IndependentWatchDogPrescaler::Div4;
		}
	case 3:
		{
			return IndependentWatchDogPrescaler::Div8;
		}
	case 4:
		{
			return IndependentWatchDogPrescaler::Div16;
		}
	case 5:
		{
			return IndependentWatchDogPrescaler::Div32;
		}
	case 6:
		{
			return IndependentWatchDogPrescaler::Div64;
		}
	case 7:
		{
			return IndependentWatchDogPrescaler::Div128;
		}
	case 8:
	default:
		{
			return IndependentWatchDogPrescaler::Div256;
		}
	}
}

bsp::IndependentWatchDogInitOptions::IndependentWatchDogInitOptions(IWDG_InitTypeDef const &o)
{
	*this = o;
}

IndependentWatchDogInitOptions &bsp::IndependentWatchDogInitOptions::operator=(IWDG_InitTypeDef const &o)
{
	_prescaler = (IndependentWatchDogPrescaler)o.Prescaler;
	_reload = o.Reload;
	return *this;
}

bsp::IndependentWatchDogInitOptions::operator IWDG_InitTypeDef() const
{
	IWDG_InitTypeDef def;
	def.Prescaler = (uint32_t)_prescaler;
	def.Reload = _reload;
	return def;
}
