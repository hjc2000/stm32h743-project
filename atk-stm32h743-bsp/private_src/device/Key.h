#pragma once
#include<bsp-interface/IKey.h>
#include<Delayer.h>
#include<hal-wrapper/peripheral/gpio/GpioPort.h>

namespace bsp
{
	class Key0 :public bsp::IKey
	{
	public:
		Key0();

		static Key0 &Instance()
		{
			static Key0 key { };
			return key;
		}

		hal::IGpioPort &Port()
		{
			return hal::GpioPortH::Instance();
		}

		static consteval hal::GpioPin Pin()
		{
			return hal::GpioPin::Pin3;
		}

		bool KeyIsDown() override;

		void Delay(std::chrono::milliseconds num) override
		{
			Delayer::Instance().Delay(num);
		}
	};

	class Key1 :public bsp::IKey
	{
	public:
		Key1();

		static Key1 &Instance()
		{
			static Key1 key { };
			return key;
		}

		hal::IGpioPort &Port()
		{
			return hal::GpioPortH::Instance();
		}

		static consteval hal::GpioPin Pin()
		{
			return hal::GpioPin::Pin2;
		}

		bool KeyIsDown() override;

		void Delay(std::chrono::milliseconds num) override
		{
			Delayer::Instance().Delay(num);
		}
	};

	class Key2 :public bsp::IKey
	{
	public:
		Key2();

		static Key2 &Instance()
		{
			static Key2 key { };
			return key;
		}

		hal::IGpioPort &Port()
		{
			return hal::GpioPortC::Instance();
		}

		static consteval hal::GpioPin Pin()
		{
			return hal::GpioPin::Pin13;
		}

		bool KeyIsDown() override;

		void Delay(std::chrono::milliseconds num) override
		{
			Delayer::Instance().Delay(num);
		}
	};
}
