#pragma once
#include <hal-wrapper/clock/clock-signal-config/ApbClkConfig.h>

namespace hal
{
    class HclkConfig
    {
    public:
        Apb3ClkConfig _apb3clk_config{};
        Apb4ClkConfig _apb4clk_config{};

        void Serialize(RCC_ClkInitTypeDef &o) const;
        void Deserialize(RCC_ClkInitTypeDef const &o);
    };
} // namespace hal
