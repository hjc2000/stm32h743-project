#pragma once
#include <hal-wrapper/clock/clock-signal-config/SystemClockConfig.h>

namespace hal
{
    /// @brief 时钟信号配置的根
    class ClockSignalConfig
    {
    public:
        ClockSignalConfig() = default;
        ClockSignalConfig(RCC_ClkInitTypeDef const &o);
        ClockSignalConfig &operator=(RCC_ClkInitTypeDef const &o);
        operator RCC_ClkInitTypeDef() const;

        enum class ClockType
        {
            SYSCLK = RCC_CLOCKTYPE_SYSCLK,
            D3PCLK1 = RCC_CLOCKTYPE_D3PCLK1,
        };

        /// @brief 选择要配置哪个时钟。
        /// @note 可以将多个枚举值按位或。也可以用 SelectAllClockType 选中全部。
        ClockType _clock_type{};

        /// @brief 选中所有时钟类型。
        void SelectAllClockType();

        /// @brief 系统时钟配置
        SystemClockConfig _system_clk_config{};

        /// @brief 读写 flash 时要延迟多少个 CPU 时钟周期。
        /// @note 因为 flash 是低速储存器，CPU 时钟频率很高时会出现速度不匹配，
        /// 这时候需要 CPU 通过延时来等待 flash。
        enum class FlashLatency
        {
            Latency0 = FLASH_LATENCY_0,
            Latency1 = FLASH_LATENCY_1,
            Latency2 = FLASH_LATENCY_2,
            Latency3 = FLASH_LATENCY_3,
            Latency4 = FLASH_LATENCY_4,
            Latency5 = FLASH_LATENCY_5,
            Latency6 = FLASH_LATENCY_6,
            Latency7 = FLASH_LATENCY_7,
        };

        FlashLatency _flash_latency = FlashLatency::Latency2;

        void Serialize(RCC_ClkInitTypeDef &o) const;
        void Deserialize(RCC_ClkInitTypeDef const &o);
    };
} // namespace hal

/// @brief 让 ClockType 枚举类型支持按位或。
/// @param left
/// @param right
/// @return
hal::ClockSignalConfig::ClockType operator|(hal::ClockSignalConfig::ClockType left,
                                            hal::ClockSignalConfig::ClockType right);
