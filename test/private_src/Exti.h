#pragma once
#include <base/define.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/interrupt/IExtiManager.h>
#include <functional>
#include <hal.h>

extern "C"
{
    void HAL_GPIO_EXTI_Callback(uint16_t pin);
}

namespace hal
{
    /// @brief 外部中断管理器
    /// @note 所有 GPIO 端口的 pin0 连接到中断管理器的 line0，
    /// 所有 GPIO 端口的 pin1 连接到中断管理器的 line1，以此类推。
    /// @note 例如同样都是 pin0，可能来自 GPIOA，GPIOB,...... 等。所有
    /// 这些端口的 pin0 都通过一个多路选择开关连接到中断管理器的 line0 上，
    /// 这个多路选择开关只能同时选择一条路导通。
    class Exti final :
        public bsp::IExtiManager
    {
    private:
        Exti();
        friend void ::HAL_GPIO_EXTI_Callback(uint16_t pin);

        std::function<void()> _on_exti0_interrupt;
        std::function<void()> _on_exti1_interrupt;
        std::function<void()> _on_exti2_interrupt;
        std::function<void()> _on_exti3_interrupt;
        std::function<void()> _on_exti4_interrupt;

    public:
        static_function Exti &Instance();

        /// @brief 注册使用一条外部中断线。本函数操作过程中会禁用全局中断。
        /// @param line_id 中断线的 id
        /// @param callback 发生中断时的回调函数
        void Register(int line_id, std::function<void()> callback) override;

        /// @brief 注销一条外部中断线的使用。本函数操作过程中会禁用全局中断。
        /// @param line_id 中断线的 id
        void Unregister(int line_id) override;
    };
} // namespace hal
