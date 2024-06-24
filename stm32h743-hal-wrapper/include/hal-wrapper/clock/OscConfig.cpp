#include "OscConfig.h"

using namespace hal;

hal::OscConfig::OscConfig(RCC_OscInitTypeDef const &o)
{
	*this = o;
}

OscConfig &OscConfig::operator=(RCC_OscInitTypeDef const &o)
{
	_oscillator_type = static_cast<OscillatorType>(o.OscillatorType);
	_hse_state = static_cast<HseState>(o.HSEState);
	_lse_state = static_cast<LseState>(o.LSEState);
	_hsi_state = static_cast<HsiState>(o.HSIState);
	_hsi_calibration_value = o.HSICalibrationValue;
	_lsi_state = static_cast<LsiState>(o.LSIState);
	_hsi_48_state = static_cast<Hsi48State>(o.HSI48State);
	_csi_state = static_cast<CsiState>(o.CSIState);
	_csi_calibration_value = o.CSICalibrationValue;
	_pll_config = hal::PllConfig{o.PLL};
	return *this;
}

OscConfig::operator RCC_OscInitTypeDef() const
{
	RCC_OscInitTypeDef o;
	o.OscillatorType = static_cast<uint32_t>(_oscillator_type);
	o.HSEState = static_cast<uint32_t>(_hse_state);
	o.LSEState = static_cast<uint32_t>(_lse_state);
	o.HSIState = static_cast<uint32_t>(_hsi_state);
	o.HSICalibrationValue = static_cast<uint32_t>(_hsi_calibration_value);
	o.LSIState = static_cast<uint32_t>(_lsi_state);
	o.HSI48State = static_cast<uint32_t>(_hsi_48_state);
	o.CSIState = static_cast<uint32_t>(_csi_state);
	o.CSICalibrationValue = static_cast<uint32_t>(_csi_calibration_value);
	o.PLL = _pll_config;
	return o;
}
