#include"HclkConfig.h"

void hal::HclkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.AHBCLKDivider = static_cast<uint32_t>(_input_divider);
	_apb1clk_config.Serialize(o);
	_apb2clk_config.Serialize(o);
	_apb3clk_config.Serialize(o);
	_apb4clk_config.Serialize(o);
}

void hal::HclkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.AHBCLKDivider);
	_apb1clk_config.Deserialize(o);
	_apb2clk_config.Deserialize(o);
	_apb3clk_config.Deserialize(o);
	_apb4clk_config.Deserialize(o);
}
