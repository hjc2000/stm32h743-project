#include"Lcd.h"
#include<atk-stm32f103/bsp.h>

using namespace bsp;
using namespace bsp;

bsp::Lcd::Lcd()
{
	InitGpio();
	__HAL_RCC_FSMC_CLK_ENABLE();

	_sram_handle.Instance = FSMC_NORSRAM_DEVICE;
	_sram_handle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	_sram_handle.Init = NorSramInitOptions();

	FSMC_NORSRAM_TimingTypeDef read_timing = ReadTiming();
	FSMC_NORSRAM_TimingTypeDef write_timing = WriteTiming();
	HAL_SRAM_Init(&_sram_handle, &read_timing, &write_timing);
}

void bsp::Lcd::InitGpio()
{
	auto init_control_line = [&]()
	{
		RD_Port().EnableClock();
		WR_Port().EnableClock();
		BL_Port().EnableClock();
		CS_Port().EnableClock();
		RS_Port().EnableClock();

		bsp::GpioPinInitOptions gpio_init_options;
		gpio_init_options._mode = GpioPinMode::AlternateFunction_PushPull;
		gpio_init_options._pull_mode = GpioPinPull::PullUp;
		gpio_init_options._speed = GpioPinSpeed::High;

		RD_Port().InitPin(RD_Pin(), gpio_init_options);
		WR_Port().InitPin(WR_Pin(), gpio_init_options);
		CS_Port().InitPin(CS_Pin(), gpio_init_options);
		RS_Port().InitPin(RS_Pin(), gpio_init_options);

		gpio_init_options._mode = GpioPinMode::Output_PushPull;
		BL_Port().InitPin(BL_Pin(), gpio_init_options);
	};

	auto init_data_bus = [&]()
	{
		bsp::GpioPortD::Instance().EnableClock();
		bsp::GpioPortE::Instance().EnableClock();

		bsp::GpioPinInitOptions options;
		options._mode = GpioPinMode::AlternateFunction_PushPull;
		options._pull_mode = GpioPinPull::PullUp;
		options._speed = GpioPinSpeed::High;

		bsp::GpioPortD::Instance().InitPin(GpioPin::Pin0, options);
		bsp::GpioPortD::Instance().InitPin(GpioPin::Pin1, options);
		bsp::GpioPortD::Instance().InitPin(GpioPin::Pin8, options);
		bsp::GpioPortD::Instance().InitPin(GpioPin::Pin9, options);
		bsp::GpioPortD::Instance().InitPin(GpioPin::Pin10, options);
		bsp::GpioPortD::Instance().InitPin(GpioPin::Pin14, options);
		bsp::GpioPortD::Instance().InitPin(GpioPin::Pin15, options);

		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin7, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin8, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin9, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin10, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin11, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin12, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin13, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin14, options);
		bsp::GpioPortE::Instance().InitPin(GpioPin::Pin15, options);
	};

	init_control_line();
	init_data_bus();
}

void bsp::Lcd::PrepareForRendering()
{
	// 写入此命令后才可以开始写像素
	WriteCommand(0X2C);
}

void bsp::Lcd::WriteCommand(uint16_t cmd)
{
	*CommandAddress() = cmd;
}

void bsp::Lcd::WriteCommand(uint16_t cmd, uint16_t param)
{
	WriteCommand(cmd);
	WriteData(param);
}

void bsp::Lcd::WriteData(uint16_t data)
{
	*DataAddress() = data;
}

uint16_t bsp::Lcd::ReadData()
{
	return *DataAddress();
}

void bsp::Lcd::TurnOnBackLight()
{
	BL_Port().DigitalWritePin(BL_Pin(), 1);
}

void bsp::Lcd::TurnOffBackLight()
{
	BL_Port().DigitalWritePin(BL_Pin(), 0);
}

uint32_t bsp::Lcd::LcdDriverChipId()
{
	uint16_t id = 0;
	WriteCommand(0X04);
	uint16_t temp = ReadData();	// 第一次读取会读取到刚刚写入的命令 0x04
	temp = ReadData();			// 读到 0x85
	temp = ReadData();			// 读到 0x85
	id = temp << 8;
	temp = ReadData();			// 读到 0x52
	id |= temp;
	return id;
}

void bsp::Lcd::DisplayOn()
{
	BSP::Delayer().Delay(std::chrono::milliseconds{ 50 });
	WriteCommand(0x11);
	BSP::Delayer().Delay(std::chrono::milliseconds{ 120 });

	WriteCommand(0x36);
	WriteData(0x00);


	WriteCommand(0x3A);
	WriteData(0X05);

	WriteCommand(0xB2);
	WriteData(0x0C);
	WriteData(0x0C);
	WriteData(0x00);
	WriteData(0x33);
	WriteData(0x33);

	WriteCommand(0xB7);
	WriteData(0x35);

	WriteCommand(0xBB); /* vcom */
	WriteData(0x32);  /* 30 */

	WriteCommand(0xC0);
	WriteData(0x0C);

	WriteCommand(0xC2);
	WriteData(0x01);

	WriteCommand(0xC3); /* vrh */
	WriteData(0x10);  /* 17 0D */

	WriteCommand(0xC4); /* vdv */
	WriteData(0x20);  /* 20 */

	WriteCommand(0xC6);
	WriteData(0x0f);

	WriteCommand(0xD0);
	WriteData(0xA4);
	WriteData(0xA1);

	WriteCommand(0xE0); /* Set Gamma  */
	WriteData(0xd0);
	WriteData(0x00);
	WriteData(0x02);
	WriteData(0x07);
	WriteData(0x0a);
	WriteData(0x28);
	WriteData(0x32);
	WriteData(0X44);
	WriteData(0x42);
	WriteData(0x06);
	WriteData(0x0e);
	WriteData(0x12);
	WriteData(0x14);
	WriteData(0x17);


	WriteCommand(0XE1);  /* Set Gamma */
	WriteData(0xd0);
	WriteData(0x00);
	WriteData(0x02);
	WriteData(0x07);
	WriteData(0x0a);
	WriteData(0x28);
	WriteData(0x31);
	WriteData(0x54);
	WriteData(0x47);
	WriteData(0x0e);
	WriteData(0x1c);
	WriteData(0x17);
	WriteData(0x1b);
	WriteData(0x1e);


	WriteCommand(0x2A);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0xef);

	WriteCommand(0x2B);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x3f);

	// 执行这条后才真正开启显示，不再是那种除了背光什么都没有的状态。
	WriteCommand(0x29);
	Clear(bsp::Color::Black);
	TurnOnBackLight();
}

void bsp::Lcd::DisplayOff()
{
	WriteCommand(0X28);
}

void bsp::Lcd::Clear(bsp::Color color)
{
	SetWindow(0, 0, Width(), Height());
	SerCursor(0, 0);
	PrepareForRendering();
	for (uint32_t i = 0; i < PointCount(); i++)
	{
		WriteData(ColorCode(color));
	}
}

void bsp::Lcd::SetScanDirection(
	bool horizontal_priority_scanning,
	bsp::HorizontalDirection hdir,
	bsp::VerticalDirection vdir
)
{
	auto direction_code = [](
		bool horizontal_priority_scanning,
		bsp::HorizontalDirection hdir,
		bsp::VerticalDirection vdir
		)
	{
		if (horizontal_priority_scanning)
		{
			if (hdir == bsp::HorizontalDirection::LeftToRight && vdir == bsp::VerticalDirection::TopToBottom)
			{
				return 0b000 << 5;
			}
			else if (hdir == bsp::HorizontalDirection::LeftToRight && vdir == bsp::VerticalDirection::BottomToTop)
			{
				return 0b100 << 5;
			}
			else if (hdir == bsp::HorizontalDirection::RightToLeft && vdir == bsp::VerticalDirection::TopToBottom)
			{
				return 0b010 << 5;
			}
			else if (hdir == bsp::HorizontalDirection::RightToLeft && vdir == bsp::VerticalDirection::BottomToTop)
			{
				return 0b110 << 5;
			}
		}

		// 以下是垂直优先扫描
		if (hdir == bsp::HorizontalDirection::LeftToRight && vdir == bsp::VerticalDirection::TopToBottom)
		{
			return 0b001 << 5;
		}
		else if (hdir == bsp::HorizontalDirection::LeftToRight && vdir == bsp::VerticalDirection::BottomToTop)
		{
			return 0b101 << 5;
		}
		else if (hdir == bsp::HorizontalDirection::RightToLeft && vdir == bsp::VerticalDirection::TopToBottom)
		{
			return 0b011 << 5;
		}

		// hdir == bsp::HorizontalDirection::RightToLeft && vdir == bsp::VerticalDirection::BottomToTop
		return 0b111 << 5;
	};

	_is_horizontal_priority_scanning = horizontal_priority_scanning;
	_horizontal_direction = hdir;
	_vertical_direction = vdir;
	WriteCommand(
		0X36,
		direction_code(horizontal_priority_scanning, hdir, vdir) | 0x8
	);

	SetWindow(0, 0, Width(), Height());
}

uint32_t bsp::Lcd::Width()
{
	if (_is_horizontal_priority_scanning)
	{
		// 如果是水平优先扫描，则屏幕当前宽度就是原始宽度
		return OriginalWidth();
	}

	// 否则将原始高度作为屏幕当前实际宽度
	return OriginHeight();
}

uint32_t bsp::Lcd::Height()
{
	if (_is_horizontal_priority_scanning)
	{
		// 如果是水平优先扫描，则将屏幕原始高度作为当前实际高度
		return OriginHeight();
	}

	// 否则将屏幕原始宽度作为当前实际高度。
	return OriginalWidth();
}

void bsp::Lcd::SetWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	WriteCommand(0X2A);
	WriteData(x >> 8);
	WriteData(x & 0xff);
	WriteData((x + width - 1) >> 8);
	WriteData((x + width - 1) & 0XFF);
	WriteCommand(0X2B);
	WriteData(y >> 8);
	WriteData(y & 0xff);
	WriteData((y + height - 1) >> 8);
	WriteData((y + height - 1) & 0XFF);
}

void bsp::Lcd::DrawPoint(uint32_t x, uint32_t y, uint16_t rgb_565)
{
	SerCursor(x, y);
	PrepareForRendering();
	WriteData(rgb_565);
}

void bsp::Lcd::SerCursor(uint32_t x, uint32_t y)
{
	WriteCommand(0X2A);
	WriteData(x >> 8);
	WriteData(x & 0XFF);
	WriteCommand(0X2B);
	WriteData(y >> 8);
	WriteData(y & 0XFF);
}
