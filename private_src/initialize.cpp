#include "initialize.h"
#include "base/Console.h"
#include "base/embedded/clock/ClockSource.h"
#include "base/embedded/cortex/mpu.h"
#include "base/embedded/extended-io/PCF8574.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/embedded/heap/heap.h"
#include "base/embedded/iic/IicHost.h"
#include "base/embedded/iic/SoftwareIicHostPinDriver.h"
#include "base/embedded/led/Led.h"
#include "base/embedded/led/LedBar.h"
#include "base/embedded/sdram/chip/w9825g6kh_6/W9825G6KH_6_Operator.h"
#include "base/embedded/sdram/SdramController.h"
#include "base/embedded/serial/Serial.h"
#include "base/stream/AsyncStreamWriter.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

void bsp::initialize_clock()
{
	/* #region hse */
	{
		base::clock::ClockSource hse{"hse"};
		hse.Configure();
	}
	/* #endregion */

	/* #region pll */
	{
		base::clock::ClockSource pll{"pll"};

		///
		/// HSE 接的是 25MHz 的晶振。
		///		@note m 是对 PLL 输入时钟的分频。25MHz 的晶振分频后是 5MHz.
		///
		/// 	@note n 是对经过 m 分频后的信号进行倍频。5MHz 被 192 倍频后是 960MHz.
		///
		/// 	@note p,q,r 是对 n 的输出进行分频。这里是 2 分频，于是得到的都是 480MHz 的时钟信号。
		/// 	其中 p 的输出作为系统时钟的输入。于是系统时钟频率是 480MHz. 系统时钟直接输入给 CPU.
		///
		std::map<std::string, uint32_t> factors{
			{"m", 5},
			{"n", 192},
			{"p", 2},
			{"q", 20},
			{"r", 2},
		};

		pll.Configure("hse", factors);
	}
	/* #endregion */

	/* #region hclk */
	{
		base::clock::ClockSource hclk{"hclk"};

		std::map<std::string, uint32_t> factors{
			{"in", 2},
		};

		hclk.Configure(factors);
	}
	/* #endregion */

	/* #region pclk1 */
	{
		base::clock::ClockSource pclk1{"pclk1"};

		std::map<std::string, uint32_t> factors{
			{"in", 2},
		};

		pclk1.Configure(factors);
	}
	/* #endregion */

	/* #region pclk2 */
	{
		base::clock::ClockSource pclk2{"pclk2"};

		std::map<std::string, uint32_t> factors{
			{"in", 2},
		};

		pclk2.Configure(factors);
	}
	/* #endregion */

	/* #region d1pclk1 */
	{
		base::clock::ClockSource d1pclk1{"d1pclk1"};

		std::map<std::string, uint32_t> factors{
			{"in", 2},
		};

		d1pclk1.Configure(factors);
	}
	/* #endregion */

	/* #region d1pclk1 */
	{
		base::clock::ClockSource d3pclk1{"d3pclk1"};

		std::map<std::string, uint32_t> factors{
			{"in", 2},
		};

		d3pclk1.Configure(factors);
	}
	/* #endregion */

	/* #region sysclk */
	{
		base::clock::ClockSource sysclk{"sysclk"};

		std::map<std::string, uint32_t> factors{
			{"out", 1},
		};

		sysclk.Configure("pll", factors);
	}
	/* #endregion */
}

void bsp::initialize_sdram_heap()
{
	base::sdram::SdramController sdram_controller{1};
	base::sdram::chip::w9825g6kh_6::W9825G6KH_6_Operator sdram{sdram_controller};
	sdram.InitializeAsReadBurstMode("hclk3", 2);

	base::cortex::mpu::configure(15,
								 reinterpret_cast<size_t>(sdram.Span().Buffer()),
								 sdram.Span().Size(),
								 base::cortex::MemoryType::NormalWithCache_WriteBack);

	base::heap::PushFront(sdram.Span());
}

void bsp::initialize_iic_host()
{
	/// @brief 连接着 EEROM 芯片和 PCF8574T 芯片的 GPIO 模拟 IIC 主机接口。
	std::shared_ptr<base::iic::SoftwareIicHostPinDriver<base::gpio::GpioPin>> pin_driver{new base::iic::SoftwareIicHostPinDriver<base::gpio::GpioPin>{
		base::gpio::GpioPin{base::gpio::PortEnum::PortH, 4},
		base::gpio::GpioPin{base::gpio::PortEnum::PortH, 5},
	}};

	std::shared_ptr<base::iic::IicHost> iic_host{new base::iic::IicHost{pin_driver}};
	base::iic::iic_host_slot().Add(iic_host);
}

void bsp::initialize_led()
{
	// 初始化 LED 灯条。
	base::led::led_bar().Add(std::vector<base::led::Led>{
		// base::led::Led{0},
		base::led::Led{1},
	});
}

void bsp::initialize_pcf8574()
{
	std::shared_ptr<base::extended_io::PCF8574> pcf8574{new base::extended_io::PCF8574{
		base::gpio::GpioPin{base::gpio::PortEnum::PortB, 12},
		base::iic::iic_host_slot()[0],
		0,
	}};

	base::extended_io::pcf8574_slot().Add(pcf8574);
}

void bsp::initialize_console()
{
	std::shared_ptr<base::serial::Serial> serial{new base::serial::Serial{1}};
	serial->Start();

	base::task::run(1024 * 2,
					[serial]()
					{
						uint8_t buffer[128]{};

						while (true)
						{
							int32_t have_read = serial->Read(base::Span{buffer, sizeof(buffer)});
							base::console().WriteLine(base::ReadOnlySpan{buffer, have_read});
							base::console().WriteLine("123456");
							base::console().WriteLine(std::to_string(have_read));
						}
					});

	std::shared_ptr<base::AsyncStreamWriter> writer{new base::AsyncStreamWriter{
		1024,
		serial,
	}};

	base::console().SetOutputWriter(writer);
}
