#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPA1 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPA1() = default;

	public:
		static_function GpioPinPA1 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPA1>
			{
			public:
				std::unique_ptr<GpioPinPA1> Create() override
				{
					return std::unique_ptr<GpioPinPA1>{new GpioPinPA1{}};
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
