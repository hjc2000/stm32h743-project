#pragma once
#include<bsp-interface/IKey.h>
#include<Delayer.h>
#include<stm32h743-hal-wrapper/peripheral/gpio/GpioPort.h>

namespace bsp
{
	/// <summary>
	///		位于 PE4 引脚上的按键。另一端接地，所以要上拉使用。
	/// </summary>
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

		bool KeyIsDown() override;

		void Delay(std::chrono::milliseconds num) override
		{
			Delayer::Instance().Delay(num);
		}
	};

	/// <summary>
	///		位于 PE3 引脚。另一端接地，所以需要上拉使用。
	/// </summary>
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

		bool KeyIsDown() override;

		void Delay(std::chrono::milliseconds num) override
		{
			Delayer::Instance().Delay(num);
		}
	};
}
