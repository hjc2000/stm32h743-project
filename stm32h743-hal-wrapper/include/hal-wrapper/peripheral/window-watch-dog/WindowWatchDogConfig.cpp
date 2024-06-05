#include"WindowWatchDogConfig.h"

using namespace hal;

WindowWatchDogConfig::WindowWatchDogConfig(WWDG_InitTypeDef const &o)
{
	*this = o;
}

WindowWatchDogConfig &WindowWatchDogConfig::operator=(WWDG_InitTypeDef const &o)
{
	_handle = o;
	return *this;
}

WWDG_InitTypeDef &hal::WindowWatchDogConfig::Handle()
{
	return _handle;
}

WindowWatchDogConfig::PrescalerOption hal::WindowWatchDogConfig::Prescaler() const
{
	return static_cast<PrescalerOption>(_handle.Prescaler);
}

void hal::WindowWatchDogConfig::SetPrescaler(PrescalerOption value)
{
	_handle.Prescaler = static_cast<uint32_t>(value);
}

uint32_t hal::WindowWatchDogConfig::Window() const
{
	return _handle.Window;
}

void hal::WindowWatchDogConfig::SetWindow(uint32_t value)
{
	_handle.Window = value;
}

uint32_t hal::WindowWatchDogConfig::CounterReloadValue() const
{
	return _handle.Counter;
}

void hal::WindowWatchDogConfig::SetCounterReloadValue(uint32_t value)
{
	_handle.Counter = value;
}

WindowWatchDogConfig::EarlyWakeupInterruptOption hal::WindowWatchDogConfig::EarlyWakeupInterrupt() const
{
	return static_cast<EarlyWakeupInterruptOption>(_handle.EWIMode);
}

void hal::WindowWatchDogConfig::SetEarlyWakeupInterrupt(EarlyWakeupInterruptOption value)
{
	_handle.EWIMode = static_cast<uint32_t>(value);
}
