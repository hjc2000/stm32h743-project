#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPE13 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPE13() = default;

	public:
		static_function GpioPinPE13 &Instance();

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
