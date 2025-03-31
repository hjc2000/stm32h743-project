#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>
#include <hal.h>

namespace bsp
{
	class GpioPinPB0 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPB0() = default;

	public:
		static_function GpioPinPB0 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPB0>
			{
			public:
				std::unique_ptr<GpioPinPB0> Create() override
				{
					return std::unique_ptr<GpioPinPB0>{new GpioPinPB0{}};
				}
			};

			Getter o;
			return o.Instance();
		}

		GPIO_TypeDef *Port() override;
		uint32_t Pin() override;

		/// @brief 引脚名称
		/// @return
		std::string PinName() const override;
	};
} // namespace bsp
