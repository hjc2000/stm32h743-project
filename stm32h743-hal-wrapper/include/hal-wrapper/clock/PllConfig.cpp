#include "PllConfig.h"

using namespace hal;

PllConfig::PllConfig(RCC_PLLInitTypeDef const &o)
{
	*this = o;
}

PllConfig &PllConfig::operator=(RCC_PLLInitTypeDef const &o)
{
	_state = static_cast<PllState>(o.PLLState);
	_source = static_cast<PllSource>(o.PLLSource);
	_m = o.PLLM;
	_n = o.PLLN;
	_p = o.PLLP;
	_q = o.PLLQ;
	_r = o.PLLR;
	_vci_range = static_cast<Pll1VciRange>(o.PLLRGE);
	_vco_range = static_cast<Pll1VcoRange>(o.PLLVCOSEL);
	_fracn = o.PLLFRACN;
	return *this;
}

PllConfig::operator RCC_PLLInitTypeDef() const
{
	RCC_PLLInitTypeDef o;
	o.PLLState = static_cast<uint32_t>(_state);
	o.PLLSource = static_cast<uint32_t>(_source);
	o.PLLM = static_cast<uint32_t>(_m);
	o.PLLN = static_cast<uint32_t>(_n);
	o.PLLP = static_cast<uint32_t>(_p);
	o.PLLQ = static_cast<uint32_t>(_q);
	o.PLLR = static_cast<uint32_t>(_r);
	o.PLLRGE = static_cast<uint32_t>(_vci_range);
	o.PLLVCOSEL = static_cast<uint32_t>(_vco_range);
	o.PLLFRACN = static_cast<uint32_t>(_fracn);
	return o;
}
