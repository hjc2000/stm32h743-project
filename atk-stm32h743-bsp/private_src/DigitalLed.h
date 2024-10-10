#pragma once
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/IDigitalLed.h>

namespace bsp
{
    /// @brief 红色 LED
    class RedDigitalLed :
        public bsp::IDigitalLed
    {
    private:
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

        bsp::IGpioPin *_pin = nullptr;

    public:
        static_function RedDigitalLed &Instance()
        {
            class Getter : public base::SingletonGetter<RedDigitalLed>
            {
            public:
                std::unique_ptr<RedDigitalLed> Create() override
                {
                    return std::unique_ptr<RedDigitalLed>{new RedDigitalLed{}};
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

        void TurnOn() override;
        void TurnOff() override;
        void Toggle() override;
    };

    /// @brief 绿色 LED
    class GreenDigitalLed : public bsp::IDigitalLed
    {
    private:
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

        bsp::IGpioPin *_pin = nullptr;

    public:
        static_function GreenDigitalLed &Instance()
        {
            class Getter : public base::SingletonGetter<GreenDigitalLed>
            {
            public:
                std::unique_ptr<GreenDigitalLed> Create() override
                {
                    return std::unique_ptr<GreenDigitalLed>{new GreenDigitalLed{}};
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

        void TurnOn() override;
        void TurnOff() override;
        void Toggle() override;
    };
} // namespace bsp
