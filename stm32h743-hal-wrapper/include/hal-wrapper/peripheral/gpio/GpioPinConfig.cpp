#include"GpioPinConfig.h"
#include<type_traits>

using namespace hal;

hal::GpioPin operator|(hal::GpioPin left, hal::GpioPin right)
{
	using T = std::underlying_type<hal::GpioPin>::type;
	T result = static_cast<T>(left) | static_cast<T>(right);
	return static_cast<hal::GpioPin>(result);
}
