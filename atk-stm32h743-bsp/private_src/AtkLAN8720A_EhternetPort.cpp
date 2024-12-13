#include "AtkLAN8720A_EhternetPort.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <base/string/ToHexString.h>
#include <base/unit/Mbps.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/system_time.h>
#include <EthernetController.h>
#include <hal.h>

bsp::AtkLAN8720A_EhternetPort &bsp::AtkLAN8720A_EhternetPort::Instance()
{
	class Getter :
		public base::SingletonGetter<AtkLAN8720A_EhternetPort>
	{
	public:
		std::unique_ptr<AtkLAN8720A_EhternetPort> Create() override
		{
			return std::unique_ptr<AtkLAN8720A_EhternetPort>{new AtkLAN8720A_EhternetPort{}};
		}

		void Lock() override
		{
			DI_DisableGlobalInterrupt();
		}

		void Unlock() override
		{
			DI_EnableGlobalInterrupt();
		}
	};

	Getter g;
	return g.Instance();
}

std::string bsp::AtkLAN8720A_EhternetPort::Name() const
{
	return "eth";
}

void bsp::AtkLAN8720A_EhternetPort::Open(base::Mac const &mac)
{
	ResetPHY();

	// 打开以太网控制器
	DI_EthernetController().Open(bsp::Ethernet_InterfaceType::RMII,
								 0,
								 mac);

	/*  SR8201F     Register 2    0x001C
					Register 3    0xC016

		YT8512C     Register 2    0x0000
					Register 3    0x0128

		LAN8720A    Register 2    0x0007
					Register 3    0xC0F0

		RTL8201BL   Register 2    0x0000
					Register 3    0x8201 */

	// ETH_CHIP_PHYSCSR = ((uint16_t)0x11);
	// ETH_CHIP_SPEED_STATUS = ((uint16_t)0x4010);
	// ETH_CHIP_DUPLEX_STATUS = ((uint16_t)0x2000);
	// PHY_TYPE = YT8512C;
	DI_Console().WriteLine("register2:" + base::ToHexString(ReadPHYRegister(2)));
	DI_Console().WriteLine("register3:" + base::ToHexString(ReadPHYRegister(3)));

	SoftwareResetPHY();
	EnableAutoNegotiation();

	// 启动以太网
	DI_EthernetController().Start(DuplexMode(), Speed());
}

uint32_t bsp::AtkLAN8720A_EhternetPort::ReadPHYRegister(uint32_t register_index)
{
	return _controller->ReadPHYRegister(register_index);
}

void bsp::AtkLAN8720A_EhternetPort::WritePHYRegister(uint32_t register_index, uint32_t value)
{
	_controller->WritePHYRegister(register_index, value);
}

void bsp::AtkLAN8720A_EhternetPort::ResetPHY()
{
	/* 公司的开发板是旧版的，复位需要先输出高电平，延时后输出低电平。
	 * 家里的开发板是新版的，复位需要先输出低电平，延时后输出高电平。
	 */

	// 硬件复位
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
	DI_Delayer().Delay(std::chrono::milliseconds{100});

	// 复位结束
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
	DI_Delayer().Delay(std::chrono::milliseconds{100});
}
