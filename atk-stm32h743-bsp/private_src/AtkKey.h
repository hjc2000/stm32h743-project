#pragma once
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/key/IKey.h>

namespace bsp
{
    /// @brief 位于 PE4 引脚上的按键。另一端接地，所以要上拉使用。
    class Key0 : public bsp::IKey
    {
    private:
        bsp::IGpioPin *_pin = nullptr;

    public:
        Key0();

        static Key0 &Instance()
        {
            static Key0 key{};
            return key;
        }

        std::string KeyName() override
        {
            return "key0";
        }

        bool KeyIsDown() override;
    };

    /// @brief 位于 PE3 引脚。另一端接地，所以需要上拉使用。
    class Key1 : public bsp::IKey
    {
    private:
        bsp::IGpioPin *_pin = nullptr;

    public:
        Key1();

        static Key1 &Instance()
        {
            static Key1 key{};
            return key;
        }

        std::string KeyName() override
        {
            return "key1";
        }

        bool KeyIsDown() override;
    };
} // namespace bsp
