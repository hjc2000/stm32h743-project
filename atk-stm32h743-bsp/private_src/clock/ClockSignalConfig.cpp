#include"ClockSignalConfig.h"
#include<type_traits>

using namespace bsp;

bsp::ClockSignalConfig::ClockSignalConfig()
{
	_clock_type = ClockType::SYSCLK
		| ClockType::HCLK
		| ClockType::PCLK1
		| ClockType::PCLK2;
}

bsp::ClockSignalConfig::ClockSignalConfig(RCC_ClkInitTypeDef const &o)
{
	*this = o;
}

ClockSignalConfig &ClockSignalConfig::operator=(RCC_ClkInitTypeDef const &o)
{
	return *this;
}

ClockSignalConfig::operator RCC_ClkInitTypeDef() const
{
	RCC_ClkInitTypeDef o;
	return o;
}

bsp::ClockSignalConfig::ClockType operator|(
	bsp::ClockSignalConfig::ClockType left,
	bsp::ClockSignalConfig::ClockType right
	)
{
	// 获取枚举类型的底层类型
	using T = std::underlying_type_t<ClockSignalConfig::ClockType>;
	T result = static_cast<T>(left) | static_cast<T>(right);
	return static_cast<ClockSignalConfig::ClockType>(result);
}
