#pragma once
#include<hal-wrapper/peripheral/gpio/IGpioPort.h>

namespace hal
{
	#pragma region GpioPortA
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
	#pragma endregion

	#pragma region GpioPortB
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
	#pragma endregion

	#pragma region GpioPortC
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
	#pragma endregion

	#pragma region GpioPortD
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
	#pragma endregion

	#pragma region GpioPortE
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
	#pragma endregion

	#pragma region GpioPortF
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
	#pragma endregion

	#pragma region GpioPortG
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
	#pragma endregion

	#pragma region GpioPortH
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
	#pragma endregion

	#pragma region GpioPortI
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
	#pragma endregion

	#pragma region GpioPortJ
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
	#pragma endregion

	#pragma region GpioPortK
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
	#pragma endregion
}
