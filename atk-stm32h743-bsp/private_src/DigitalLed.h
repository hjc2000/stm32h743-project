#pragma once
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/IDigitalLed.h>

namespace bsp
{
    /// @brief 红色 LED
    class RedDigitalLed :
        public bsp::IDigitalLed
    {
    private:
        bsp::IGpioPin *_pin = nullptr;

    public:
        RedDigitalLed()
        {
            auto options = DICreate_GpioPinOptions();
            options->SetDirection(bsp::IGpioPinDirection::Output);
            options->SetDriver(bsp::IGpioPinDriver::PushPull);
            options->SetPullMode(bsp::IGpioPinPullMode::PullUp);
            options->SetSpeedLevel(3);
            options->SetWorkMode(bsp::IGpioPinWorkMode::Gpio);

            _pin = DI_GpioPinCollection().Get("PB1");
            _pin->Open(*options);

            TurnOff();
        }

        static RedDigitalLed &Instance()
        {
            static RedDigitalLed led;
            return led;
        }

        void TurnOn() override;
        void TurnOff() override;
        void Toggle() override;
    };

    /// @brief 绿色 LED
    class GreenDigitalLed : public bsp::IDigitalLed
    {
    private:
        bsp::IGpioPin *_pin = nullptr;

    public:
        GreenDigitalLed()
        {
            auto options = DICreate_GpioPinOptions();
            options->SetDirection(bsp::IGpioPinDirection::Output);
            options->SetDriver(bsp::IGpioPinDriver::PushPull);
            options->SetPullMode(bsp::IGpioPinPullMode::PullUp);
            options->SetSpeedLevel(3);
            options->SetWorkMode(bsp::IGpioPinWorkMode::Gpio);

            _pin = DI_GpioPinCollection().Get("PB0");
            _pin->Open(*options);

            TurnOff();
        }

        static GreenDigitalLed &Instance()
        {
            static GreenDigitalLed led;
            return led;
        }

        void TurnOn() override;
        void TurnOff() override;
        void Toggle() override;
    };
} // namespace bsp
