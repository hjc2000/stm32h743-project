#pragma once
#include <hal.h>

namespace hal
{
    class Apb3ClkConfig
    {
    public:
        enum class InputDivider
        {
            DIV1 = RCC_APB3_DIV1,
            DIV2 = RCC_APB3_DIV2,
            DIV4 = RCC_APB3_DIV4,
            DIV8 = RCC_APB3_DIV8,
            DIV16 = RCC_APB3_DIV16,
        };

        InputDivider _input_divider{};

        void Serialize(RCC_ClkInitTypeDef &o) const;
        void Deserialize(RCC_ClkInitTypeDef const &o);
    };

    class Apb4ClkConfig
    {
    public:
        enum class InputDivider
        {
            DIV1 = RCC_APB4_DIV1,
            DIV2 = RCC_APB4_DIV2,
            DIV4 = RCC_APB4_DIV4,
            DIV8 = RCC_APB4_DIV8,
            DIV16 = RCC_APB4_DIV16,
        };

        InputDivider _input_divider{};

        void Serialize(RCC_ClkInitTypeDef &o) const;
        void Deserialize(RCC_ClkInitTypeDef const &o);
    };
} // namespace hal
