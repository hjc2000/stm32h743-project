#pragma once
#include <base/define.h>
#include <bsp-interface/clock/IClockSource.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <hal.h>

namespace bsp
{
    class PllClockSource :
        public bsp::IClockSource
    {
    private:
        bool _opened = false;
        IClockSource_State _state = IClockSource_State::Off;
        base::MHz _p_freq;
        base::MHz _q_freq;
        base::MHz _r_freq;

    public:
        static_function PllClockSource &Instance();

        /// @brief 本时钟源的名称。
        /// @return
        std::string Name() const override;

#pragma region Open
        /// @brief 用户自己决定输入通道和各个分频、倍频系数。
        /// @param input_channel_name 输入通道名。
        /// @param factor 分频、倍频系数都放在这个字典里，使用不同的名称来区分是什么。
        void Open(std::string const &input_channel_name,
                  base::IDictionary<std::string, int> const &factors) override;
#pragma endregion

        /// @brief 关闭时钟源。
        void Close() override;

        /// @brief 本时钟源当前的状态。
        /// @return
        IClockSource_State State() const override;

        /// @brief 查看某一个输出通道的频率。有的时钟源会有多个输出通道。
        /// @param output_channel_name 输出通道名。
        /// @return
        base::MHz Frequency(std::string const &output_channel_name) const override;
    };
} // namespace bsp
