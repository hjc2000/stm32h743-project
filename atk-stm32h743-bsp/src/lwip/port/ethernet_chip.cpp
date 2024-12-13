#include "ethernet_chip.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/ethernet.h>

#if PHY_AUTO_SELECT

#else
int PHY_TYPE;
uint16_t ETH_CHIP_SPEED_STATUS;
uint16_t ETH_CHIP_DUPLEX_STATUS;
#endif

int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);

/**
  * @brief       获取ETH_CHIP设备的链路状态
  * @param       pobj: 设备对象
  * @param       pLinkState: 指向链路状态的指针
  * @retval      ETH_CHIP_STATUS_100MBITS_FULLDUPLEX：100M，全双工
				 ETH_CHIP_STATUS_100MBITS_HALFDUPLEX ：100M，半双工
				 ETH_CHIP_STATUS_10MBITS_FULLDUPLEX：10M，全双工
				 ETH_CHIP_STATUS_10MBITS_HALFDUPLEX ：10M，半双工
				 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
  */
int32_t eth_chip_get_link_state()
{
	uint32_t readval = 0;

	/* 检测特殊功能寄存器链接值 */
	if (ETH_PHY_IO_ReadReg(0, 0x1F, &readval) < 0)
	{
		return ETH_CHIP_STATUS_READ_ERROR;
	}

	if (((readval & ETH_CHIP_SPEED_STATUS) != ETH_CHIP_SPEED_STATUS) && ((readval & ETH_CHIP_DUPLEX_STATUS) != 0))
	{
		return ETH_CHIP_STATUS_100MBITS_FULLDUPLEX;
	}
	else if (((readval & ETH_CHIP_SPEED_STATUS) != ETH_CHIP_SPEED_STATUS))
	{
		return ETH_CHIP_STATUS_100MBITS_HALFDUPLEX;
	}
	else if (((readval & ETH_CHIP_BCR_DUPLEX_MODE) != ETH_CHIP_BCR_DUPLEX_MODE))
	{
		return ETH_CHIP_STATUS_10MBITS_FULLDUPLEX;
	}
	else
	{
		return ETH_CHIP_STATUS_10MBITS_HALFDUPLEX;
	}
}

/**
  * @brief       设置ETH_CHIP设备的链路状态
  * @param       pobj: 设备对象
  * @param       pLinkState: 指向链路状态的指针
  * @retval      ETH_CHIP_STATUS_OK：设置成功
				 ETH_CHIP_STATUS_ERROR ：设置失败
				 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
				 ETH_CHIP_STATUS_WRITE_ERROR ：不能写入寄存器
  */
int32_t eth_chip_set_link_state(uint32_t linkstate)
{
	uint32_t bcrvalue = 0;
	int32_t status = ETH_CHIP_STATUS_OK;

	if (ETH_PHY_IO_ReadReg(0, ETH_CHIP_BCR, &bcrvalue) >= 0)
	{
		/* 禁用链路配置(自动协商，速度和双工) */
		bcrvalue &= ~(ETH_CHIP_BCR_AUTONEGO_EN | ETH_CHIP_BCR_SPEED_SELECT | ETH_CHIP_BCR_DUPLEX_MODE);

		if (linkstate == ETH_CHIP_STATUS_100MBITS_FULLDUPLEX)
		{
			bcrvalue |= (ETH_CHIP_BCR_SPEED_SELECT | ETH_CHIP_BCR_DUPLEX_MODE);
		}
		else if (linkstate == ETH_CHIP_STATUS_100MBITS_HALFDUPLEX)
		{
			bcrvalue |= ETH_CHIP_BCR_SPEED_SELECT;
		}
		else if (linkstate == ETH_CHIP_STATUS_10MBITS_FULLDUPLEX)
		{
			bcrvalue |= ETH_CHIP_BCR_DUPLEX_MODE;
		}
		else
		{
			/* 错误的链路状态参数 */
			status = ETH_CHIP_STATUS_ERROR;
		}
	}
	else
	{
		status = ETH_CHIP_STATUS_READ_ERROR;
	}

	if (status == ETH_CHIP_STATUS_OK)
	{
		/* 写入链路状态 */
		if (ETH_PHY_IO_WriteReg(0, ETH_CHIP_BCR, bcrvalue) < 0)
		{
			status = ETH_CHIP_STATUS_WRITE_ERROR;
		}
	}

	return status;
}

/**
 * @breif       获得网络芯片的速度模式
 * @param       无
 * @retval      1:100M
				0:10M
 */
uint8_t ethernet_chip_get_speed(void)
{
	uint8_t speed = 0;
	if (PHY_TYPE == LAN8720)
	{
		DI_Console().WriteLine("LAN8720");

		/* 从LAN8720的31号寄存器中读取网络速度和双工模式 */
		speed = ~((DI_EthernetController().ReadPHYRegister(0x1F) & ETH_CHIP_SPEED_STATUS));
	}
	else if (PHY_TYPE == SR8201F)
	{
		DI_Console().WriteLine("SR8201F");

		/* 从SR8201F的0号寄存器中读取网络速度和双工模式 */
		speed = ((DI_EthernetController().ReadPHYRegister(0x1F) & ETH_CHIP_SPEED_STATUS) >> 13);
	}
	else if (PHY_TYPE == YT8512C)
	{
		DI_Console().WriteLine("YT8512C");

		/* 从YT8512C的17号寄存器中读取网络速度和双工模式 */
		speed = ((DI_EthernetController().ReadPHYRegister(0x1F) & ETH_CHIP_SPEED_STATUS) >> 14);
	}
	else if (PHY_TYPE == RTL8201)
	{
		DI_Console().WriteLine("RTL8201");

		/* 从RTL8201的16号寄存器中读取网络速度和双工模式 */
		speed = ((DI_EthernetController().ReadPHYRegister(0x1F) & ETH_CHIP_SPEED_STATUS) >> 1);
	}

	return speed;
}
