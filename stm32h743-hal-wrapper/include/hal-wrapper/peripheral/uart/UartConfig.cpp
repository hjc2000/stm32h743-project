#include"UartConfig.h"

using namespace hal;

hal::UartConfig::UartConfig(UART_InitTypeDef const &o)
{
	*this = o;
}

UartConfig &hal::UartConfig::operator=(UART_InitTypeDef const &o)
{
	_baud_rate = o.BaudRate;
	_word_length = (WordLength)o.WordLength;
	_stop_bit_count = (StopBitCount)o.StopBits;
	_parity = (Parity)o.Parity;
	_mode = (Mode)o.Mode;
	_hardware_flow_control = (HardwareFlowControl)o.HwFlowCtl;
	_over_sampling = (OverSampling)o.OverSampling;
	return *this;
}

hal::UartConfig::operator UART_InitTypeDef() const
{
	UART_InitTypeDef o;
	o.BaudRate = _baud_rate;
	o.WordLength = (uint32_t)_word_length;
	o.StopBits = (uint32_t)_stop_bit_count;
	o.Parity = (uint32_t)_parity;
	o.Mode = (uint32_t)_mode;
	o.HwFlowCtl = (uint32_t)_hardware_flow_control;
	o.OverSampling = (uint32_t)_over_sampling;
	return o;
}
