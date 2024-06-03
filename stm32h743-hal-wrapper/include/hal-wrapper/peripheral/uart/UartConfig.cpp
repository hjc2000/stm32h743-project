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

void hal::UartConfig::Deserialize(bsp::ISerial const &serial)
{
	_baud_rate = serial.BaudRate();

	switch (serial.DataBits())
	{
	case 7:
		{
			_word_length = WordLength::WordLength_7bit;
			break;
		}
	case 8:
		{
			_word_length = WordLength::WordLength_8bit;
			break;
		}
	case 9:
		{
			_word_length = WordLength::WordLength_9bit;
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
			_stop_bit_count = StopBitCount::StopBitCount_0_5_bit;
			break;
		}
	case bsp::ISerial::StopBitsOption::One:
		{
			_stop_bit_count = StopBitCount::StopBitCount_1_bit;
			break;
		}
	case bsp::ISerial::StopBitsOption::OnePointFive:
		{
			_stop_bit_count = StopBitCount::StopBitCount_1_5_bit;
			break;
		}
	case bsp::ISerial::StopBitsOption::Tow:
		{
			_stop_bit_count = StopBitCount::StopBitCount_2_bit;
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
			_parity = Parity::None;
			break;
		}
	case bsp::ISerial::ParityOption::Even:
		{
			_parity = Parity::Even;
			break;
		}
	case bsp::ISerial::ParityOption::Odd:
		{
			_parity = Parity::Odd;
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
			_hardware_flow_control = HardwareFlowControl::None;
			break;
		}
	case bsp::ISerial::HardwareFlowControlOption::CTS:
		{
			_hardware_flow_control = HardwareFlowControl::CTS;
			break;
		}
	case bsp::ISerial::HardwareFlowControlOption::RTS:
		{
			_hardware_flow_control = HardwareFlowControl::RTS;
			break;
		}
	case bsp::ISerial::HardwareFlowControlOption::RTS_CTS:
		{
			_hardware_flow_control = HardwareFlowControl::RTS_CTS;
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
	case WordLength::WordLength_7bit:
		{
			serial.SetDataBits(7);
			break;
		}
	case WordLength::WordLength_8bit:
		{
			serial.SetDataBits(8);
			break;
		}
	case WordLength::WordLength_9bit:
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
	case StopBitCount::StopBitCount_0_5_bit:
		{
			serial.SetStopBits(bsp::ISerial::StopBitsOption::ZeroPointFive);
			break;
		}
	case StopBitCount::StopBitCount_1_bit:
		{
			serial.SetStopBits(bsp::ISerial::StopBitsOption::One);
			break;
		}
	case StopBitCount::StopBitCount_1_5_bit:
		{
			serial.SetStopBits(bsp::ISerial::StopBitsOption::OnePointFive);
			break;
		}
	case StopBitCount::StopBitCount_2_bit:
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
	case Parity::None:
		{
			serial.SetParity(bsp::ISerial::ParityOption::None);
			break;
		}
	case Parity::Even:
		{
			serial.SetParity(bsp::ISerial::ParityOption::Even);
			break;
		}
	case Parity::Odd:
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
	case HardwareFlowControl::None:
		{
			serial.SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption::None);
			break;
		}
	case HardwareFlowControl::CTS:
		{
			serial.SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption::CTS);
			break;
		}
	case HardwareFlowControl::RTS:
		{
			serial.SetHardwareFlowControl(bsp::ISerial::HardwareFlowControlOption::RTS);
			break;
		}
	case HardwareFlowControl::RTS_CTS:
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
