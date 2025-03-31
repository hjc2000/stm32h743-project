#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPB12 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPB12() = default;

	public:
		static_function GpioPinPB12 &Instance();

		GPIO_TypeDef *Port() override;
		uint32_t Pin() override;

		/// @brief 引脚名称
		/// @return
		std::string PinName() const override;
	};
} // namespace bsp
