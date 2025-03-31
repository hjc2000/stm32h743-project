#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPG12 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPG12() = default;

	public:
		static_function GpioPinPG12 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPG12>
			{
			public:
				std::unique_ptr<GpioPinPG12> Create() override
				{
					return std::unique_ptr<GpioPinPG12>{new GpioPinPG12{}};
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
