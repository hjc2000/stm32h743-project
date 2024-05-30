#include"ApbClkConfig.h"

using namespace hal;

void hal::Apb1ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB1CLKDivider = static_cast<uint32_t>(_input_divider);
}

void hal::Apb1ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB1CLKDivider);
}

void hal::Apb2ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB2CLKDivider = static_cast<uint32_t>(_input_divider);
}

void hal::Apb2ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB2CLKDivider);
}

void hal::Apb3ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB3CLKDivider = static_cast<uint32_t>(_input_divider);
}

void hal::Apb3ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB3CLKDivider);
}

void hal::Apb4ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB4CLKDivider = static_cast<uint32_t>(_input_divider);
}

void hal::Apb4ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB4CLKDivider);
}
