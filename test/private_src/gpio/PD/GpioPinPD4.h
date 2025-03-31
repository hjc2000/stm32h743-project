#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPD4 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPD4() = default;

	public:
		static_function GpioPinPD4 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPD4>
			{
			public:
				std::unique_ptr<GpioPinPD4> Create() override
				{
					return std::unique_ptr<GpioPinPD4>{new GpioPinPD4{}};
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
