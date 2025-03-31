#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPH3 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPH3() = default;

	public:
		static_function GpioPinPH3 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPH3>
			{
			public:
				std::unique_ptr<GpioPinPH3> Create() override
				{
					return std::unique_ptr<GpioPinPH3>{new GpioPinPH3{}};
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
