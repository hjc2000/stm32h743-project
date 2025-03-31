#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPB5 final : public bsp::GpioPin
	{
	private:
		GpioPinPB5() = default;

	public:
		static_function GpioPinPB5 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPB5>
			{
			public:
				std::unique_ptr<GpioPinPB5> Create() override
				{
					return std::unique_ptr<GpioPinPB5>{new GpioPinPB5{}};
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
