#include"OscConfig.h"

using namespace bsp;

bsp::OscConfig::OscConfig(RCC_OscInitTypeDef const &def)
{
	*this = def;
}

OscConfig &OscConfig::operator=(RCC_OscInitTypeDef const &value)
{
	_oscillator_type = (OscillatorType)value.OscillatorType;
	_hse_state = (HseState)value.HSEState;
	_hse_prediv = (HsePrediv)value.HSEPredivValue;
	_lse_state = (LseState)value.LSEState;
	_hsi_state = (HsiState)value.HSIState;
	_hsi_calibration_value = value.HSICalibrationValue;
	_lsi_state = (LsiState)value.LSIState;
	_pll_init_options = value.PLL;
	return *this;
}

OscConfig::operator RCC_OscInitTypeDef() const
{
	RCC_OscInitTypeDef def;
	def.OscillatorType = (uint32_t)_oscillator_type;
	def.HSEState = (uint32_t)_hse_state;
	def.HSEPredivValue = (uint32_t)_hse_prediv;
	def.LSEState = (uint32_t)_lse_state;
	def.HSIState = (uint32_t)_hsi_state;
	def.HSICalibrationValue = _hsi_calibration_value;
	def.LSIState = (uint32_t)_lsi_state;
	def.PLL = _pll_init_options;
	return def;
}
