#pragma once

namespace bsp
{
	void initialize_clock();

	void initialize_sdram_heap();

	void initialize_iic_host();

	void initialize_led();

	void initialize_pcf8574();

	void initialize_console();

} // namespace bsp
