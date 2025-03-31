#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>
#include <hal.h>

namespace bsp
{
	class GpioPinPB1 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPB1() = default;

	public:
		static_function GpioPinPB1 &Instance()
		{
			class Getter :
				public bsp::TaskSingletonGetter<GpioPinPB1>
			{
			public:
				std::unique_ptr<GpioPinPB1> Create() override
				{
					return std::unique_ptr<GpioPinPB1>{new GpioPinPB1{}};
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

		/// @brief 将引脚打开为复用功能。
		/// @param function_name 复用功能名称。
		/// @param pull_mode 拉模式。
		/// @param driver_mode 驱动模式。
		void OpenAsAlternateFunctionMode(std::string function_name,
										 bsp::IGpioPinPullMode pull_mode,
										 bsp::IGpioPinDriver driver_mode) override;
	};
} // namespace bsp
