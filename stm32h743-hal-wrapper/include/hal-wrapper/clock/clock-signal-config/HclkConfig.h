#pragma once
#include <hal-wrapper/clock/clock-signal-config/ApbClkConfig.h>

namespace hal
{
    class HclkConfig
    {
    public:
#pragma region 子时钟
        Apb2ClkConfig _apb2clk_config{};
        Apb3ClkConfig _apb3clk_config{};
        Apb4ClkConfig _apb4clk_config{};
#pragma endregion

        void Serialize(RCC_ClkInitTypeDef &o) const;
        void Deserialize(RCC_ClkInitTypeDef const &o);
    };
} // namespace hal
