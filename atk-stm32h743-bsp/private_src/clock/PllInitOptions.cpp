#include"PllInitOptions.h"

using namespace bsp;

PllInitOptions &PllInitOptions::operator=(RCC_PLLInitTypeDef const &value)
{
	_state = (PllState)value.PLLState;
	_clock_source = (PllClockSource)value.PLLSource;
	_mul = (PllMul)value.PLLMUL;
	return *this;
}

PllInitOptions::operator RCC_PLLInitTypeDef() const
{
	RCC_PLLInitTypeDef def;
	def.PLLState = (uint32_t)_state;
	def.PLLSource = (uint32_t)_clock_source;
	def.PLLMUL = (uint32_t)_mul;
	return def;
}
