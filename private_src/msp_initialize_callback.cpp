#include "base/embedded/gpio/GpioPin.h"
#include "base/embedded/sdram/sdram_controller_handle.h"
#include "hal.h" // IWYU pragma: keep
#include <cstdint>
#include <vector>

void base::sdram::msp_initialize_callback(uint32_t id)
{
	std::vector<base::gpio::GpioPin> pins{
		base::gpio::GpioPin{base::gpio::PortEnum::PortC, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortC, 2},
		base::gpio::GpioPin{base::gpio::PortEnum::PortC, 3},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 8},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 9},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 10},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 14},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 15},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 7},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 8},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 9},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 10},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 11},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 12},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 13},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 14},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 15},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 2},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 3},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 4},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 5},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 11},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 12},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 13},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 14},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 15},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 2},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 4},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 5},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 8},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 15},
	};

	for (auto &pin : pins)
	{
		pin.InitializeAsAlternateFunctionMode(GPIO_AF12_FMC,
											  base::gpio::PullMode::PullUp,
											  base::gpio::DriveMode::PushPull);
	}
}
