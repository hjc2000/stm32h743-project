#pragma once
#include <atomic>
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/systick.h>
#include <bsp-interface/key/IEventDrivenKey.h>

namespace bsp
{
    /// @brief 外部中断驱动的按钮，按钮的丝印是 wake up。
    class ExtiWakeUpKey : public bsp::IEventDrivenKey
    {
    private:
        ExtiWakeUpKey();

        /// @brief 中断函数中经过消抖，确认按键被按下后就会将此字段赋值为 true。
        std::atomic_bool _is_pressed = false;
        bsp::IGpioPin *_pin = nullptr;

    public:
        static_function ExtiWakeUpKey &Instance()
        {
            class Getter : public base::SingletonGetter<ExtiWakeUpKey>
            {
            public:
                std::unique_ptr<ExtiWakeUpKey> Create() override
                {
                    return std::unique_ptr<ExtiWakeUpKey>{new ExtiWakeUpKey{}};
                }

                void Lock() override
                {
                    DI_DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }

        bool IsPressed() override
        {
            return _is_pressed;
        }

        void ClearPressedFlag() override
        {
            _is_pressed = false;
        }
    };
} // namespace bsp
