#include"IndependentWatchDogConfig.h"
#include<stdexcept>

using namespace hal;

hal::IndependentWatchDogConfig::IndependentWatchDogConfig()
{
	SetWindowValue(0x0FFF);
}

IndependentWatchDogConfig::IndependentWatchDogConfig(IWDG_InitTypeDef const &o)
{
	*this = o;
}

IndependentWatchDogConfig &IndependentWatchDogConfig::operator=(IWDG_InitTypeDef const &o)
{
	_config_handle = o;
	return *this;
}

IWDG_InitTypeDef &IndependentWatchDogConfig::Handle()
{
	return _config_handle;
}

#pragma region 分频系数
IndependentWatchDogConfig::PrescalerOption IndependentWatchDogConfig::Prescaler() const
{
	return static_cast<PrescalerOption>(_config_handle.Prescaler);
}

void IndependentWatchDogConfig::SetPrescaler(PrescalerOption value)
{
	_config_handle.Prescaler = static_cast<uint32_t>(value);
}

uint8_t IndependentWatchDogConfig::GetPrescalerByPow() const
{
	switch (Prescaler())
	{
	case PrescalerOption::Div4:
		{
			return 2;
		}
	case PrescalerOption::Div8:
		{
			return 3;
		}
	case PrescalerOption::Div16:
		{
			return 4;
		}
	case PrescalerOption::Div32:
		{
			return 5;
		}
	case PrescalerOption::Div64:
		{
			return 6;
		}
	case PrescalerOption::Div128:
		{
			return 7;
		}
	case PrescalerOption::Div256:
		{
			return 8;
		}
	default:
		{
			throw std::invalid_argument { "Prescaler() 的值非法" };
		}
	}
}

void IndependentWatchDogConfig::SetPrescalerByPow(uint8_t pow)
{
	auto power_to_prescaler = [](uint8_t pow)->IndependentWatchDogConfig::PrescalerOption
	{
		switch (pow)
		{
		case 2:
			{
				return PrescalerOption::Div4;
			}
		case 3:
			{
				return PrescalerOption::Div8;
			}
		case 4:
			{
				return PrescalerOption::Div16;
			}
		case 5:
			{
				return PrescalerOption::Div32;
			}
		case 6:
			{
				return PrescalerOption::Div64;
			}
		case 7:
			{
				return PrescalerOption::Div128;
			}
		case 8:
			{
				return PrescalerOption::Div256;
			}
		default:
			{
				throw std::invalid_argument { "pow 的值非法" };
			}
		}
	};

	SetPrescaler(power_to_prescaler(pow));
}

uint32_t IndependentWatchDogConfig::GetPrescalerByUint32() const
{
	switch (Prescaler())
	{
	case PrescalerOption::Div4:
		{
			return 4;
		}
	case PrescalerOption::Div8:
		{
			return 8;
		}
	case PrescalerOption::Div16:
		{
			return 16;
		}
	case PrescalerOption::Div32:
		{
			return 32;
		}
	case PrescalerOption::Div64:
		{
			return 64;
		}
	case PrescalerOption::Div128:
		{
			return 128;
		}
	case PrescalerOption::Div256:
		{
			return 256;
		}
	default:
		{
			throw std::invalid_argument { "Prescaler() 的值非法" };
		}
	}
}

void IndependentWatchDogConfig::SetPrescalerByUint32(uint32_t value)
{
	switch (value)
	{
	case 4:
		{
			SetPrescaler(PrescalerOption::Div4);
			break;
		}
	case 8:
		{
			SetPrescaler(PrescalerOption::Div8);
			break;
		}
	case 16:
		{
			SetPrescaler(PrescalerOption::Div16);
			break;
		}
	case 32:
		{
			SetPrescaler(PrescalerOption::Div32);
			break;
		}
	case 64:
		{
			SetPrescaler(PrescalerOption::Div64);
			break;
		}
	case 128:
		{
			SetPrescaler(PrescalerOption::Div128);
			break;
		}
	case 256:
		{
			SetPrescaler(PrescalerOption::Div256);
			break;
		}
	default:
		{
			throw std::invalid_argument { "SetPrescalerByUint32 的 value 非法" };
		}
	}
}
#pragma endregion

uint32_t IndependentWatchDogConfig::ReloadValue() const
{
	return _config_handle.Reload;
}

void IndependentWatchDogConfig::SetReloadValue(uint32_t value)
{
	_config_handle.Reload = value;
}

uint32_t hal::IndependentWatchDogConfig::WindowValue() const
{
	return _config_handle.Window;
}

void hal::IndependentWatchDogConfig::SetWindowValue(uint32_t value)
{
	_config_handle.Window = value;
}
