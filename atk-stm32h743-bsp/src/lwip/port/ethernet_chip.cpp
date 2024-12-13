#include "ethernet_chip.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/ethernet.h>

int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);

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
