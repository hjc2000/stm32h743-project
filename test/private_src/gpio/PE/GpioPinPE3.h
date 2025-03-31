#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPE3 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPE3() = default;

	public:
		static_function GpioPinPE3 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPE3>
			{
			public:
				std::unique_ptr<GpioPinPE3> Create() override
				{
					return std::unique_ptr<GpioPinPE3>{new GpioPinPE3{}};
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
