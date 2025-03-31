#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPE5 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPE5() = default;

	public:
		static_function GpioPinPE5 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPE5>
			{
			public:
				std::unique_ptr<GpioPinPE5> Create() override
				{
					return std::unique_ptr<GpioPinPE5>{new GpioPinPE5{}};
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
