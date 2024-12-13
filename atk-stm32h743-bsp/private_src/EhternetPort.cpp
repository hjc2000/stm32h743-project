#include "EhternetPort.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <base/unit/Mbps.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/system_time.h>
#include <EthernetController.h>
#include <hal.h>

int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);

void bsp::EhternetPort::ChipInitialize()
{
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

	// 软件复位
	WritePHYRegister(0, 0x8000U);
	base::Seconds now = DI_SystemTime();
	while (true)
	{
		if (static_cast<int64_t>(DI_SystemTime() - now) > 2)
		{
			throw std::runtime_error{"软件复位 PHY 超时"};
		}

		uint32_t register_value = ReadPHYRegister(0);
		if (!(register_value & 0x8000U))
		{
			break;
		}
	}

	/* 到了这里，初始化完成！！！ */
	DI_Delayer().Delay(std::chrono::seconds{2});
}

bsp::EhternetPort &bsp::EhternetPort::Instance()
{
	class Getter :
		public base::SingletonGetter<EhternetPort>
	{
	public:
		std::unique_ptr<EhternetPort> Create() override
		{
			return std::unique_ptr<EhternetPort>{new EhternetPort{}};
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

std::string bsp::EhternetPort::Name() const
{
	return "eth";
}

void bsp::EhternetPort::Open(base::Mac const &mac)
{
	ResetPHY();

	// 打开以太网控制器
	DI_EthernetController().Open(bsp::Ethernet_InterfaceType::RMII,
								 0,
								 mac);

	ChipInitialize();
	EnableAutoNegotiation();

	// 启动以太网
	DI_EthernetController().Start(DuplexMode(), Speed());
}

uint32_t bsp::EhternetPort::ReadPHYRegister(uint32_t register_index)
{
	return _controller->ReadPHYRegister(register_index);
}

void bsp::EhternetPort::WritePHYRegister(uint32_t register_index, uint32_t value)
{
	_controller->WritePHYRegister(register_index, value);
}

void bsp::EhternetPort::ResetPHY()
{
	/* 公司的开发板是旧版的，复位需要先输出高电平，延时后输出低电平。
	 * 家里的开发板是新版的，复位需要先输出低电平，延时后输出高电平。
	 */

	// 硬件复位
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
	DI_Delayer().Delay(std::chrono::milliseconds{100});

	// 复位结束
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
	DI_Delayer().Delay(std::chrono::milliseconds{100});
}

void bsp::EhternetPort::EnableAutoNegotiation()
{
	uint32_t bcr = ReadPHYRegister(static_cast<uint32_t>(PhyRegister::BCR));
	bcr |= 0x1000U;
	WritePHYRegister(static_cast<uint32_t>(PhyRegister::BCR), bcr);
}

void bsp::EhternetPort::EnablePowerDownMode()
{
	uint32_t register_value = ReadPHYRegister(0);
	register_value |= 0x0800U;
	WritePHYRegister(0, register_value);
}

void bsp::EhternetPort::DisablePowerDownMode()
{
	uint32_t register_value = ReadPHYRegister(0);
	register_value &= ~0x0800U;
	WritePHYRegister(0, register_value);
}

void bsp::EhternetPort::EnableLoopbackMode()
{
	uint32_t register_value = ReadPHYRegister(0);
	register_value |= 0x4000U;
	WritePHYRegister(0, register_value);
}

void bsp::EhternetPort::DisableLoopbackMode()
{
	uint32_t register_value = ReadPHYRegister(0);
	register_value &= ~0x4000U;
	WritePHYRegister(0, register_value);
}

bsp::Ethernet_DuplexMode bsp::EhternetPort::DuplexMode()
{
	uint32_t register_value = ReadPHYRegister(0x1F);
	uint32_t const mask = 0b10000;
	if (register_value & mask)
	{
		return bsp::Ethernet_DuplexMode::FullDuplex;
	}

	return bsp::Ethernet_DuplexMode::HalfDuplex;
}

base::Bps bsp::EhternetPort::Speed()
{
	uint32_t register_value = ReadPHYRegister(0x1F);
	uint32_t const mask = 0b01000;
	if (register_value & mask)
	{
		return base::Mbps{100};
	}

	return base::Mbps{10};
}
