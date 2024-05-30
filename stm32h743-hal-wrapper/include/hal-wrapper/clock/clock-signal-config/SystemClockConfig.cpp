#include"SystemClockConfig.h"

void hal::SystemClockConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.SYSCLKSource = static_cast<uint32_t>(_clock_source);
	o.SYSCLKDivider = static_cast<uint32_t>(_output_divider);
	_hclk_config.Serialize(o);
}

void hal::SystemClockConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_clock_source = static_cast<decltype(_clock_source)>(o.SYSCLKSource);
	_output_divider = static_cast<decltype(_output_divider)>(o.SYSCLKDivider);
	_hclk_config.Deserialize(o);
}
