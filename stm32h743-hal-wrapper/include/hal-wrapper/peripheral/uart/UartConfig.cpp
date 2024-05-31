#include"UartConfig.h"

using namespace hal;

hal::UartConfig::UartConfig(UART_InitTypeDef const &o)
{
	*this = o;
}

UartConfig &hal::UartConfig::operator=(UART_InitTypeDef const &o)
{
	_baud_rate = o.BaudRate;
	_word_length = static_cast<WordLength>(o.WordLength);
	_stop_bit_count = static_cast<StopBitCount>(o.StopBits);
	_parity = static_cast<Parity>(o.Parity);
	_mode = static_cast<Mode>(o.Mode);
	_hardware_flow_control = static_cast<HardwareFlowControl>(o.HwFlowCtl);
	_over_sampling = static_cast<OverSampling>(o.OverSampling);
	_one_bit_sampling = static_cast<OneBitSampling>(o.OneBitSampling);
	_clock_prescaler = static_cast<ClockPrescaler>(o.ClockPrescaler);
	return *this;
}

hal::UartConfig::operator UART_InitTypeDef() const
{
	UART_InitTypeDef o;
	o.BaudRate = _baud_rate;
	o.WordLength = static_cast<uint32_t>(_word_length);
	o.StopBits = static_cast<uint32_t>(_stop_bit_count);
	o.Parity = static_cast<uint32_t>(_parity);
	o.Mode = static_cast<uint32_t>(_mode);
	o.HwFlowCtl = static_cast<uint32_t>(_hardware_flow_control);
	o.OverSampling = static_cast<uint32_t>(_over_sampling);
	o.OneBitSampling = static_cast<uint32_t>(_one_bit_sampling);
	o.ClockPrescaler = static_cast<uint32_t>(_clock_prescaler);
	return o;
}
