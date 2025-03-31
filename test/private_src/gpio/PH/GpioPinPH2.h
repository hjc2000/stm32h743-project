#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPH2 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPH2() = default;

	public:
		static_function GpioPinPH2 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPH2>
			{
			public:
				std::unique_ptr<GpioPinPH2> Create() override
				{
					return std::unique_ptr<GpioPinPH2>{new GpioPinPH2{}};
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
