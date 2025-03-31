#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPD11 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPD11() = default;

	public:
		static_function GpioPinPD11 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPD11>
			{
			public:
				std::unique_ptr<GpioPinPD11> Create() override
				{
					return std::unique_ptr<GpioPinPD11>{new GpioPinPD11{}};
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
