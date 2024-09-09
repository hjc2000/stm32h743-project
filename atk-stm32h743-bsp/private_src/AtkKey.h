#pragma once
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>
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
            class Getter : public base::SingletonGetter<Key0>
            {
            public:
                std::unique_ptr<Key0> Create() override
                {
                    return std::unique_ptr<Key0>{new Key0{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
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
            class Getter : public base::SingletonGetter<Key1>
            {
            public:
                std::unique_ptr<Key1> Create() override
                {
                    return std::unique_ptr<Key1>{new Key1{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }

        std::string KeyName() override
        {
            return "key1";
        }

        bool KeyIsDown() override;
    };
} // namespace bsp
