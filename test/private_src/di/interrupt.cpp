#include <bsp-interface/di/interrupt.h>
#include <Exti.h>
#include <hal.h>

/// @brief 禁用 irq 指定的中断。
/// @param irq
void bsp::di::interrupt::DisableInterrupt(uint32_t irq) noexcept
{
    HAL_NVIC_DisableIRQ(static_cast<IRQn_Type>(irq));
}

/// @brief 启用 irq 指定的中断。
/// @param irq
void bsp::di::interrupt::EnableInterrupt(uint32_t irq) noexcept
{
    /* 硬件中断控制器中每一个中断都天生自带一个默认的优先级，并且这种优先级一般是由中断号的大小
     * 自然而然地就决定了。所以这里只要使能中断就行了，使用硬件中断控制器自带的优先级。
     */
    HAL_NVIC_EnableIRQ(static_cast<IRQn_Type>(irq));
}

/// @brief 使能指定 irq 的中断。
/// @param irq 中断号。
/// @param priority 优先级。
void bsp::di::interrupt::EnableInterrupt(uint32_t irq, uint32_t priority) noexcept
{
    // 中断优先级可能需要先禁用中断后才能改。不知道，反正做了没错。
    bsp::di::interrupt::DisableInterrupt(irq);
    HAL_NVIC_SetPriority(static_cast<IRQn_Type>(irq), priority, 0);
    HAL_NVIC_EnableIRQ(static_cast<IRQn_Type>(irq));
}

namespace
{
    int32_t volatile _disable_global_interrupt_times = 0;
}

/// @brief 禁止全局中断
void bsp::di::interrupt::DisableGlobalInterrupt() noexcept
{
    __disable_irq();
    _disable_global_interrupt_times = _disable_global_interrupt_times + 1;
}

/// @brief 启用全局中断
void bsp::di::interrupt::EnableGlobalInterrupt() noexcept
{
    __disable_irq();
    _disable_global_interrupt_times = _disable_global_interrupt_times - 1;
    if (_disable_global_interrupt_times <= 0)
    {
        _disable_global_interrupt_times = 0;
        __enable_irq();
    }
}

/// @brief 全局中断是否被禁止了。
/// @return
bool bsp::di::interrupt::GlobalInterruptIsDisabled() noexcept
{
    return _disable_global_interrupt_times > 0;
}

bsp::IExtiManager &bsp::di::interrupt::ExtiManager()
{
    return hal::Exti::Instance();
}
