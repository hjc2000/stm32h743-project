#include"ClockSignalConfig.h"
#include<type_traits>

using namespace hal;

void hal::ClockSignalConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.ClockType = static_cast<uint32_t>(_clock_type);
	_system_clk_config.Serialize(o);
}

void hal::ClockSignalConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_clock_type = static_cast<decltype(_clock_type)>(o.ClockType);
	_system_clk_config.Deserialize(o);
}

hal::ClockSignalConfig::ClockSignalConfig(RCC_ClkInitTypeDef const &o)
{
	*this = o;
}

ClockSignalConfig &ClockSignalConfig::operator=(RCC_ClkInitTypeDef const &o)
{
	Deserialize(o);
	return *this;
}

ClockSignalConfig::operator RCC_ClkInitTypeDef() const
{
	RCC_ClkInitTypeDef o;
	Serialize(o);
	return o;
}

void hal::ClockSignalConfig::SelectAllClockType()
{
	_clock_type = ClockType::SYSCLK
		| ClockType::HCLK
		| ClockType::D1PCLK1
		| ClockType::PCLK1
		| ClockType::PCLK2
		| ClockType::D3PCLK1;
}

hal::ClockSignalConfig::ClockType operator|(
	hal::ClockSignalConfig::ClockType left,
	hal::ClockSignalConfig::ClockType right
	)
{
	// 获取枚举类型的底层类型
	using T = std::underlying_type_t<ClockSignalConfig::ClockType>;
	T result = static_cast<T>(left) | static_cast<T>(right);
	return static_cast<ClockSignalConfig::ClockType>(result);
}

