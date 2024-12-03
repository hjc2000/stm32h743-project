#include "HclkConfig.h"

void hal::HclkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
    _apb4clk_config.Serialize(o);
}

void hal::HclkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
    _apb4clk_config.Deserialize(o);
}
