#pragma once
#include <bsp-interface/key/IKey.h>
#include <hal-wrapper/peripheral/gpio/GpioPort.h>

namespace bsp
{
	class Key0 : public bsp::IKey
	{
	public:
		Key0();

		static Key0 &Instance()
		{
			static Key0 key{};
			return key;
		}

		std::string KeyName() override
		{
			return "key0";
		}

		hal::IGpioPort &Port()
		{
			return hal::GpioPortH::Instance();
		}

		static consteval hal::GpioPinConfig::PinEnum Pin()
		{
			return hal::GpioPinConfig::PinEnum::Pin3;
		}

		bool KeyIsDown() override;
	};

	class Key1 : public bsp::IKey
	{
	public:
		Key1();

		static Key1 &Instance()
		{
			static Key1 key{};
			return key;
		}

		std::string KeyName() override
		{
			return "key1";
		}

		hal::IGpioPort &Port()
		{
			return hal::GpioPortH::Instance();
		}

		static consteval hal::GpioPinConfig::PinEnum Pin()
		{
			return hal::GpioPinConfig::PinEnum::Pin2;
		}

		bool KeyIsDown() override;
	};

	class Key2 : public bsp::IKey
	{
	public:
		Key2();

		static Key2 &Instance()
		{
			static Key2 key{};
			return key;
		}

		std::string KeyName() override
		{
			return "key2";
		}

		hal::IGpioPort &Port()
		{
			return hal::GpioPortC::Instance();
		}

		static consteval hal::GpioPinConfig::PinEnum Pin()
		{
			return hal::GpioPinConfig::PinEnum::Pin13;
		}

		bool KeyIsDown() override;
	};

	class KeyWakeUp : public bsp::IKey
	{
	public:
		KeyWakeUp();

		static KeyWakeUp &Instance()
		{
			static KeyWakeUp key{};
			return key;
		}

		std::string KeyName() override
		{
			return "key_wake_up";
		}

		hal::IGpioPort &Port()
		{
			return hal::GpioPortA::Instance();
		}

		static consteval hal::GpioPinConfig::PinEnum Pin()
		{
			return hal::GpioPinConfig::PinEnum::Pin0;
		}

		bool KeyIsDown() override;
	};
}
