#include"ClockSignalConfig.h"
#include<type_traits>

using namespace bsp;

void bsp::Apb1ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB1CLKDivider = static_cast<uint32_t>(_input_divider);
}

void bsp::Apb1ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB1CLKDivider);
}

void bsp::Apb2ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB2CLKDivider = static_cast<uint32_t>(_input_divider);
}

void bsp::Apb2ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB2CLKDivider);
}

void bsp::Apb3ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB3CLKDivider = static_cast<uint32_t>(_input_divider);
}

void bsp::Apb3ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB3CLKDivider);
}

void bsp::Apb4ClkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.APB4CLKDivider = static_cast<uint32_t>(_input_divider);
}

void bsp::Apb4ClkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.APB4CLKDivider);
}

void bsp::HclkConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.AHBCLKDivider = static_cast<uint32_t>(_input_divider);
	_apb1clk_config.Serialize(o);
	_apb2clk_config.Serialize(o);
	_apb3clk_config.Serialize(o);
	_apb4clk_config.Serialize(o);
}

void bsp::HclkConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_input_divider = static_cast<decltype(_input_divider)>(o.AHBCLKDivider);
	_apb1clk_config.Deserialize(o);
	_apb2clk_config.Deserialize(o);
	_apb3clk_config.Deserialize(o);
	_apb4clk_config.Deserialize(o);
}

void bsp::SystemClockConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.SYSCLKSource = static_cast<uint32_t>(_clock_source);
	o.SYSCLKDivider = static_cast<uint32_t>(_output_divider);
	_hclk_config.Serialize(o);
}

void bsp::SystemClockConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_clock_source = static_cast<decltype(_clock_source)>(o.SYSCLKSource);
	_output_divider = static_cast<decltype(_output_divider)>(o.SYSCLKDivider);
	_hclk_config.Deserialize(o);
}

void bsp::ClockSignalConfig::Serialize(RCC_ClkInitTypeDef &o) const
{
	o.ClockType = static_cast<uint32_t>(_clock_type);
	_system_clk_config.Serialize(o);
}

void bsp::ClockSignalConfig::Deserialize(RCC_ClkInitTypeDef const &o)
{
	_clock_type = static_cast<decltype(_clock_type)>(o.ClockType);
	_system_clk_config.Deserialize(o);
}

bsp::ClockSignalConfig::ClockSignalConfig(RCC_ClkInitTypeDef const &o)
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

void bsp::ClockSignalConfig::SelectAllClockType()
{
	_clock_type = ClockType::SYSCLK
		| ClockType::HCLK
		| ClockType::D1PCLK1
		| ClockType::PCLK1
		| ClockType::PCLK2
		| ClockType::D3PCLK1;
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

