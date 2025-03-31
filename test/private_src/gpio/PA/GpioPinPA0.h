#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPA0 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPA0() = default;

	public:
		static_function GpioPinPA0 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPA0>
			{
			public:
				std::unique_ptr<GpioPinPA0> Create() override
				{
					return std::unique_ptr<GpioPinPA0>{new GpioPinPA0{}};
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
