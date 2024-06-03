#include"UartConfig.h"
#include<stdexcept>

using namespace hal;

hal::UartConfig::UartConfig(UART_InitTypeDef const &o)
{
	*this = o;
}

UartConfig &hal::UartConfig::operator=(UART_InitTypeDef const &o)
{
	_baud_rate = o.BaudRate;
	_word_length = static_cast<WordLengthOption>(o.WordLength);
	_stop_bit_count = static_cast<StopBitsOption>(o.StopBits);
	_parity = static_cast<ParityOption>(o.Parity);
	_mode = static_cast<ModeOption>(o.Mode);
	_hardware_flow_control = static_cast<HardwareFlowControlOption>(o.HwFlowCtl);
	_over_sampling = static_cast<OverSamplingOption>(o.OverSampling);
	_one_bit_sampling = static_cast<OneBitSamplingOption>(o.OneBitSampling);
	_clock_prescaler = static_cast<ClockPrescalerOption>(o.ClockPrescaler);
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

void hal::UartConfig::Deserialize(bsp::ISerial const &serial)
{
	_baud_rate = serial.BaudRate();

	switch (serial.DataBits())
	{
	case 7:
		{
			_word_length = WordLengthOption::WordLength_7bit;
			break;
		}
	case 8:
		{
			_word_length = WordLengthOption::WordLength_8bit;
			break;
		}
	case 9:
		{
			_word_length = WordLengthOption::WordLength_9bit;
			break;
		}
	default:
		{
			throw std::invalid_argument { "serial.DataBits() 的值非法。" };
		}
	}

	switch (serial.StopBits())
	{
	case bsp::ISerial::StopBitsOption::ZeroPointFive:
		{
			_stop_bit_count = StopBitsOption::StopBitCount_0_5_bit;
			break;
		}
	case bsp::ISerial::StopBitsOption::One:
		{
			_stop_bit_count = StopBitsOption::StopBitCount_1_bit;
			break;
		}
	case bsp::ISerial::StopBitsOption::OnePointFive:
		{
			_stop_bit_count = StopBitsOption::StopBitCount_1_5_bit;
			break;
		}
	case bsp::ISerial::StopBitsOption::Tow:
		{
			_stop_bit_count = StopBitsOption::StopBitCount_2_bit;
			break;
		}
	default:
		{
			throw std::invalid_argument { "serial.StopBits() 非法" };
		}
	}

	switch (serial.Parity())
	{
	case bsp::ISerial::ParityOption::None:
		{
			_parity = ParityOption::None;
			break;
		}
	case bsp::ISerial::ParityOption::Even:
		{
			_parity = ParityOption::Even;
			break;
		}
	case bsp::ISerial::ParityOption::Odd:
		{
			_parity = ParityOption::Odd;
			break;
		}
	default:
		{
			throw std::invalid_argument { "serial.Parity() 非法" };
		}
	}

	switch (serial.HardwareFlowControl())
	{
	case bsp::ISerial::HardwareFlowControlOption::None:
		{
			_hardware_flow_control = HardwareFlowControlOption::None;
			break;
		}
	case bsp::ISerial::HardwareFlowControlOption::CTS:
		{
			_hardware_flow_control = HardwareFlowControlOption::CTS;
			break;
		}
	case bsp::ISerial::HardwareFlowControlOption::RTS:
		{
			_hardware_flow_control = HardwareFlowControlOption::RTS;
			break;
		}
	case bsp::ISerial::HardwareFlowControlOption::RTS_CTS:
		{
			_hardware_flow_control = HardwareFlowControlOption::RTS_CTS;
			break;
		}
	default:
		{
			throw std::invalid_argument { "serial.HardwareFlowControl() 的值非法" };
		}
	}
}

void hal::UartConfig::Serialize(bsp::ISerial &serial) const
{
	serial.SetBaudRate(_baud_rate);

	switch (_word_length)
	{
	case WordLengthOption::WordLength_7bit:
		{
			serial.SetDataBits(7);
			break;
		}
	case WordLengthOption::WordLength_8bit:
		{
			serial.SetDataBits(8);
			break;
		}
	case WordLengthOption::WordLength_9bit:
		{
			serial.SetDataBits(9);
			break;
		}
	default:
		{
			throw std::invalid_argument { "_word_length 的值非法" };
		}
	}

	switch (_stop_bit_count)
	{
	case StopBitsOption::StopBitCount_0_5_bit:
		{
			serial.SetStopBits(bsp::ISerial::StopBitsOption::ZeroPointFive);
			break;
		}
	case StopBitsOption::StopBitCount_1_bit:
		{
			serial.SetStopBits(bsp::ISerial::StopBitsOption::One);
			break;
		}
	case StopBitsOption::StopBitCount_1_5_bit:
		{
			serial.SetStopBits(bsp::ISerial::StopBitsOption::OnePointFive);
			break;
		}
	case StopBitsOption::StopBitCount_2_bit:
		{
			serial.SetStopBits(bsp::ISerial::StopBitsOption::Tow);
			break;
		}
	default:
		{
			throw std::invalid_argument { "_stop_bit_count 的值非法" };
		}
	}

	switch (_parity)
	{
	case ParityOption::None:
		{
			serial.SetParity(bsp::ISerial::ParityOption::None);
			break;
		}
	case ParityOption::Even:
		{
			serial.SetParity(bsp::ISerial::ParityOption::Even);
			break;
		}
	case ParityOption::Odd:
		{
			serial.SetParity(bsp::ISerial::ParityOption::Odd);
			break;
		}
	default:
		{
			throw std::invalid_argument { "_parity 的值非法" };
		}
	}

	switch (_hardware_flow_control)
	{
	case HardwareFlowControlOption::None:
		{
			serial.SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption::None);
			break;
		}
	case HardwareFlowControlOption::CTS:
		{
			serial.SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption::CTS);
			break;
		}
	case HardwareFlowControlOption::RTS:
		{
			serial.SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption::RTS);
			break;
		}
	case HardwareFlowControlOption::RTS_CTS:
		{
			serial.SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption::RTS_CTS);
			break;
		}
	default:
		{
			throw std::invalid_argument { "_hardware_flow_control 的值非法" };
		}
	}
}
