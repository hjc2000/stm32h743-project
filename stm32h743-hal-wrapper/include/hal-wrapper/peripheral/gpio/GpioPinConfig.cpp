#include "GpioPinConfig.h"
#include <type_traits>

using namespace hal;

hal::GpioPinConfig::PinEnum operator|(hal::GpioPinConfig::PinEnum left,
									  hal::GpioPinConfig::PinEnum right)
{
	using T = std::underlying_type<hal::GpioPinConfig::PinEnum>::type;
	T result = static_cast<T>(left) | static_cast<T>(right);
	return static_cast<hal::GpioPinConfig::PinEnum>(result);
}
