#include "EhternetPort.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <ethernet_chip.h>
#include <EthernetController.h>
#include <hal.h>

int32_t ETH_PHY_IO_Init(void);
int32_t ETH_PHY_IO_DeInit(void);
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);
int32_t ETH_PHY_IO_GetTick(void);

extern eth_chip_object_t ETHCHIP;

void bsp::EhternetPort::ChipInitialize()
{
	uint32_t tickstart = 0, regvalue = 0, addr = 0;
	int32_t status = ETH_CHIP_STATUS_OK;

	/*  SR8201F     Register 2    0x001C
					Register 3    0xC016

		YT8512C     Register 2    0x0000
					Register 3    0x0128

		LAN8720A    Register 2    0x0007
					Register 3    0xC0F0

		RTL8201BL   Register 2    0x0000
					Register 3    0x8201 */
	regvalue = DI_EthernetController().ReadPHYRegister(PHY_REGISTER2);

	switch (regvalue)
	{
	case YT8512C_AND_RTL8201BL_PHYREGISTER2:
		{
			regvalue = DI_EthernetController().ReadPHYRegister(PHY_REGISTER3);
			if (regvalue == 0x128)
			{
				ETH_CHIP_PHYSCSR = ((uint16_t)0x11);
				ETH_CHIP_SPEED_STATUS = ((uint16_t)0x4010);
				ETH_CHIP_DUPLEX_STATUS = ((uint16_t)0x2000);
				PHY_TYPE = YT8512C;
			}
			else
			{
				ETH_CHIP_PHYSCSR = ((uint16_t)0x10);
				ETH_CHIP_SPEED_STATUS = ((uint16_t)0x0022);
				ETH_CHIP_DUPLEX_STATUS = ((uint16_t)0x0004);
				PHY_TYPE = RTL8201;
			}

			break;
		}
	case SR8201F_PHYREGISTER2:
		{
			ETH_CHIP_PHYSCSR = ((uint16_t)0x00);
			ETH_CHIP_SPEED_STATUS = ((uint16_t)0x2020);
			ETH_CHIP_DUPLEX_STATUS = ((uint16_t)0x0100);
			PHY_TYPE = SR8201F;
			break;
		}
	case LAN8720A_PHYREGISTER2:
		{
			ETH_CHIP_PHYSCSR = ((uint16_t)0x1F);
			ETH_CHIP_SPEED_STATUS = ((uint16_t)0x0004);
			ETH_CHIP_DUPLEX_STATUS = ((uint16_t)0x0010);
			PHY_TYPE = LAN8720;
			break;
		}
	}

	if (ETHCHIP.is_initialized == 0)
	{
		/* MDC时钟 */
		ETH_PHY_IO_Init();

		/* 设置PHY地址为32 */
		ETHCHIP.devaddr = ETH_CHIP_MAX_DEV_ADDR + 1;

		/* 主要为了查找PHY地址 */
		for (addr = 0; addr <= ETH_CHIP_MAX_DEV_ADDR; addr++)
		{
			if (ETH_PHY_IO_ReadReg(addr, ETH_CHIP_PHYSCSR, &regvalue) < 0)
			{
				status = ETH_CHIP_STATUS_READ_ERROR;
				/* 无法读取这个设备地址继续下一个地址 */
				continue;
			}

			/* 已经找到PHY地址了 */
			if ((regvalue & ETH_CHIP_PHY_COUNT) == addr)
			{
				ETHCHIP.devaddr = addr;
				status = ETH_CHIP_STATUS_OK;
				break;
			}
		}

		/* 判断这个PHY地址是否大于32（2^5）*/
		if (ETHCHIP.devaddr > ETH_CHIP_MAX_DEV_ADDR)
		{
			status = ETH_CHIP_STATUS_ADDRESS_ERROR;
		}

		/* 如果PHY地址有效 */
		if (status == ETH_CHIP_STATUS_OK)
		{
			/* 设置软件复位  */
			if (ETH_PHY_IO_WriteReg(ETHCHIP.devaddr, ETH_CHIP_BCR, ETH_CHIP_BCR_SOFT_RESET) >= 0)
			{
				/* 获取软件重置状态 */
				if (ETH_PHY_IO_ReadReg(ETHCHIP.devaddr, ETH_CHIP_BCR, &regvalue) >= 0)
				{
					tickstart = ETH_PHY_IO_GetTick();

					/* 等待软件复位完成或超时  */
					while (regvalue & ETH_CHIP_BCR_SOFT_RESET)
					{
						if ((ETH_PHY_IO_GetTick() - tickstart) <= ETH_CHIP_SW_RESET_TO)
						{
							if (ETH_PHY_IO_ReadReg(ETHCHIP.devaddr, ETH_CHIP_BCR, &regvalue) < 0)
							{
								status = ETH_CHIP_STATUS_READ_ERROR;
								break;
							}
						}
						else
						{
							status = ETH_CHIP_STATUS_RESET_TIMEOUT;
							break;
						}
					}
				}
				else
				{
					status = ETH_CHIP_STATUS_READ_ERROR;
				}
			}
			else
			{
				status = ETH_CHIP_STATUS_WRITE_ERROR;
			}
		}
	}

	/* 到了这里，初始化完成！！！ */
	if (status == ETH_CHIP_STATUS_OK)
	{
		tickstart = ETH_PHY_IO_GetTick();

		/* 等待2s进行初始化 */
		while ((ETH_PHY_IO_GetTick() - tickstart) <= ETH_CHIP_INIT_TO)
		{
		}

		ETHCHIP.is_initialized = 1;
	}
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
	DI_EthernetController().Open(bsp::IEthernetController_InterfaceType::RMII,
								 0,
								 mac);

	ChipInitialize();
	EnableAutoNegotiation();
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
	bcr |= ETH_CHIP_BCR_AUTONEGO_EN;
	WritePHYRegister(static_cast<uint32_t>(PhyRegister::BCR), bcr);
}
