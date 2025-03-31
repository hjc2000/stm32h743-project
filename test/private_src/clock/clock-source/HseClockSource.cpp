#include "HseClockSource.h"

bsp::HseClockSource &bsp::HseClockSource::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<HseClockSource>
    {
    public:
        std::unique_ptr<HseClockSource> Create() override
        {
            return std::unique_ptr<HseClockSource>{new HseClockSource{}};
        }
    };

    Getter g;
    return g.Instance();
}

std::string bsp::HseClockSource::Name() const
{
    return "hse";
}

void bsp::HseClockSource::Open(base::MHz const &crystal_frequency)
{
    RCC_OscInitTypeDef def{};
    def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    def.HSEState = RCC_HSE_ON;
    HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
    if (result != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"打开 hse 时钟源失败。"};
    }

    _frequency = crystal_frequency;
    _state = IClockSource_State::On;
}

void bsp::HseClockSource::SetAsBypass(base::MHz external_clock_frequency)
{
    RCC_OscInitTypeDef def{};
    def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    def.HSEState = RCC_HSE_BYPASS;

    HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
    if (result != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"设置 hse 时钟源为旁路失败。"};
    }

    _frequency = external_clock_frequency;
    _state = IClockSource_State::Bypass;
}

void bsp::HseClockSource::Close()
{
    RCC_OscInitTypeDef def{};
    def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    def.HSEState = RCC_HSE_OFF;

    HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
    if (result != HAL_StatusTypeDef::HAL_OK)
    {
        throw std::runtime_error{"关闭 hse 时钟源失败。"};
    }

    _frequency = base::MHz{0};
    _state = IClockSource_State::Off;
}

bsp::IClockSource_State bsp::HseClockSource::State() const
{
    return _state;
}

base::MHz bsp::HseClockSource::Frequency() const
{
    return _frequency;
}
