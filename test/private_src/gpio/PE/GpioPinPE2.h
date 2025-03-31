#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPE2 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPE2() = default;

	public:
		static_function GpioPinPE2 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPE2>
			{
			public:
				std::unique_ptr<GpioPinPE2> Create() override
				{
					return std::unique_ptr<GpioPinPE2>{new GpioPinPE2{}};
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
