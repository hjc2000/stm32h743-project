#include <cstdint>

extern "C"
{
	uint32_t stm32_hal_get_hse_crystal_oscillator_frequency()
	{
		return static_cast<uint32_t>(25e6);
	}
}
