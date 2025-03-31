#include "Exti.h"
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <stdexcept>

hal::Exti::Exti()
{
    bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn),
                                            []()
                                            {
                                                HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
                                            });

    bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn),
                                            []()
                                            {
                                                HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
                                            });

    bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn),
                                            []()
                                            {
                                                HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
                                            });

    bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn),
                                            []()
                                            {
                                                HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
                                            });

    bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn),
                                            []()
                                            {
                                                HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
                                            });
}

hal::Exti &hal::Exti::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<Exti>
    {
    public:
        std::unique_ptr<Exti> Create() override
        {
            return std::unique_ptr<Exti>{new Exti{}};
        }
    };

    Getter g;
    return g.Instance();
}

void hal::Exti::Register(int line_id, std::function<void()> callback)
{
    bsp::di::interrupt::GlobalInterruptGuard g;
    switch (line_id)
    {
    case 0:
        {
            bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn), 4);
            _on_exti0_interrupt = callback;
            break;
        }
    case 1:
        {
            bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn), 4);
            _on_exti1_interrupt = callback;
            break;
        }
    case 2:
        {
            bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn), 4);
            _on_exti2_interrupt = callback;
            break;
        }
    case 3:
        {
            bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn), 4);
            _on_exti3_interrupt = callback;
            break;
        }
    case 4:
        {
            bsp::di::interrupt::EnableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn), 4);
            _on_exti4_interrupt = callback;
            break;
        }
    default:
        {
            throw std::invalid_argument{"pin 超出范围。"};
        }
    }
}

void hal::Exti::Unregister(int line_id)
{
    bsp::di::interrupt::GlobalInterruptGuard g;
    switch (line_id)
    {
    case 0:
        {
            bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn));
            _on_exti0_interrupt = nullptr;
            break;
        }
    case 1:
        {
            bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn));
            _on_exti1_interrupt = nullptr;
            break;
        }
    case 2:
        {
            bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn));
            _on_exti2_interrupt = nullptr;
            break;
        }
    case 3:
        {
            bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn));
            _on_exti3_interrupt = nullptr;
            break;
        }
    case 4:
        {
            bsp::di::interrupt::DisableInterrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn));
            _on_exti4_interrupt = nullptr;
            break;
        }
    default:
        {
            throw std::invalid_argument{"pin 超出范围。"};
        }
    }
}

extern "C"
{
    /// @brief 重写 HAL 库中的 weak 版本
    /// @param pin
    void HAL_GPIO_EXTI_Callback(uint16_t pin)
    {
        switch (pin)
        {
        case GPIO_PIN_0:
            {
                if (hal::Exti::Instance()._on_exti0_interrupt)
                {
                    hal::Exti::Instance()._on_exti0_interrupt();
                }

                break;
            }
        case GPIO_PIN_1:
            {
                if (hal::Exti::Instance()._on_exti1_interrupt)
                {
                    hal::Exti::Instance()._on_exti1_interrupt();
                }

                break;
            }
        case GPIO_PIN_2:
            {
                if (hal::Exti::Instance()._on_exti2_interrupt)
                {
                    hal::Exti::Instance()._on_exti2_interrupt();
                }

                break;
            }
        case GPIO_PIN_3:
            {
                if (hal::Exti::Instance()._on_exti3_interrupt)
                {
                    hal::Exti::Instance()._on_exti3_interrupt();
                }

                break;
            }
        case GPIO_PIN_4:
            {
                if (hal::Exti::Instance()._on_exti4_interrupt)
                {
                    hal::Exti::Instance()._on_exti4_interrupt();
                }

                break;
            }
        default:
            {
                break;
            }
        }
    }
} // extern "C"
