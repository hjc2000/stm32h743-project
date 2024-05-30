#pragma once
#include<hal-wrapper/peripheral/gpio/IGpioPort.h>

namespace hal
{
	#ifdef GPIOA
	class GpioPortA :public IGpioPort
	{
	public:
		static GpioPortA &Instance()
		{
			static GpioPortA port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOA

	#ifdef GPIOB
	class GpioPortB :public IGpioPort
	{
	public:
		static GpioPortB &Instance()
		{
			static GpioPortB port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOB

	#ifdef GPIOC
	class GpioPortC :public IGpioPort
	{
	public:
		static GpioPortC &Instance()
		{
			static GpioPortC port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOC

	#ifdef GPIOD
	class GpioPortD :public IGpioPort
	{
	public:
		static GpioPortD &Instance()
		{
			static GpioPortD port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOD

	#ifdef GPIOE
	class GpioPortE :public IGpioPort
	{
	public:
		static GpioPortE &Instance()
		{
			static GpioPortE port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOE

	#ifdef GPIOF
	class GpioPortF :public IGpioPort
	{
	public:
		static GpioPortF &Instance()
		{
			static GpioPortF port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOF

	#ifdef GPIOG
	class GpioPortG :public IGpioPort
	{
	public:
		static GpioPortG &Instance()
		{
			static GpioPortG port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOG

	#ifdef GPIOH
	class GpioPortH :public IGpioPort
	{
	public:
		static GpioPortH &Instance()
		{
			static GpioPortH port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOH

	#ifdef GPIOI
	class GpioPortI :public IGpioPort
	{
	public:
		static GpioPortI &Instance()
		{
			static GpioPortI port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOI

	#ifdef GPIOJ
	class GpioPortJ :public IGpioPort
	{
	public:
		static GpioPortJ &Instance()
		{
			static GpioPortJ port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOJ

	#ifdef GPIOK
	class GpioPortK :public IGpioPort
	{
	public:
		static GpioPortK &Instance()
		{
			static GpioPortK port { };
			return port;
		}

		GPIO_TypeDef *HardwareInstance() override;
		bool IsClockEnabled() override;
		void EnableClock() override;
		void DisableClock() override;
	};
	#endif // GPIOK
}