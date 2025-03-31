#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPD5 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPD5() = default;

	public:
		static_function GpioPinPD5 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPD5>
			{
			public:
				std::unique_ptr<GpioPinPD5> Create() override
				{
					return std::unique_ptr<GpioPinPD5>{new GpioPinPD5{}};
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
