#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPC1 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPC1() = default;

		bool _is_open = false;

	public:
		static_function GpioPinPC1 &Instance();

		GPIO_TypeDef *Port() override;
		uint32_t Pin() override;

		/// @brief 引脚名称
		/// @return
		std::string PinName() const override;
	};
} // namespace bsp
