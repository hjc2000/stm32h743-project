#include "HclkConfig.h"

void hal::HclkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
    _apb1clk_config.Serialize(o);
    _apb2clk_config.Serialize(o);
    _apb3clk_config.Serialize(o);
    _apb4clk_config.Serialize(o);
}

void hal::HclkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
    _apb1clk_config.Deserialize(o);
    _apb2clk_config.Deserialize(o);
    _apb3clk_config.Deserialize(o);
    _apb4clk_config.Deserialize(o);
}
