/**
 ****************************************************************************************************
 * @file        ethernet_chip.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       PHY芯片配置驱动
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 H743开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20220420
 * 第一次发布
 *
 ****************************************************************************************************
 */
#include "ethernet_chip.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/ethernet.h>

#define ETH_CHIP_SW_RESET_TO ((uint32_t)500U) /* 软件复位等待时间 */
#define ETH_CHIP_INIT_TO ((uint32_t)2000U)    /* 初始化等待时间 */
#define ETH_CHIP_MAX_DEV_ADDR ((uint32_t)31U) /* PHY地址的最大值 */

#if PHY_AUTO_SELECT

#else
#define YT8512C_AND_RTL8201BL_PHYREGISTER2 0x0000
#define SR8201F_PHYREGISTER2 0x001C
#define LAN8720A_PHYREGISTER2 0x0007
int PHY_TYPE;
uint16_t ETH_CHIP_PHYSCSR;
uint16_t ETH_CHIP_SPEED_STATUS;
uint16_t ETH_CHIP_DUPLEX_STATUS;
#endif

int32_t ETH_PHY_IO_Init(void);
int32_t ETH_PHY_IO_DeInit(void);
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);
int32_t ETH_PHY_IO_GetTick(void);

/**
  * @brief       初始化ETH_CHIP并配置所需的硬件资源
  * @param       pobj: 设备对象
  * @retval      ETH_CHIP_STATUS_OK：初始化ETH_CHIP并配置所需的硬件资源成功
                 ETH_CHIP_STATUS_ADDRESS_ERROR：找不到设备地址
                 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
                 ETH_CHIP_STATUS_WRITE_ERROR：不能写入寄存器
                 ETH_CHIP_STATUS_RESET_TIMEOUT：无法执行软件复位
  */
int32_t eth_chip_init(eth_chip_object_t *pobj)
{
    uint32_t tickstart = 0, regvalue = 0, addr = 0;
    int32_t status = ETH_CHIP_STATUS_OK;

#if PHY_AUTO_SELECT

#else
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
    case SR8201F_PHYREGISTER2:
        ETH_CHIP_PHYSCSR = ((uint16_t)0x00);
        ETH_CHIP_SPEED_STATUS = ((uint16_t)0x2020);
        ETH_CHIP_DUPLEX_STATUS = ((uint16_t)0x0100);
        PHY_TYPE = SR8201F;
        break;
    case LAN8720A_PHYREGISTER2:
        ETH_CHIP_PHYSCSR = ((uint16_t)0x1F);
        ETH_CHIP_SPEED_STATUS = ((uint16_t)0x0004);
        ETH_CHIP_DUPLEX_STATUS = ((uint16_t)0x0010);
        PHY_TYPE = LAN8720;
        break;
    }
#endif

    if (pobj->is_initialized == 0)
    {
        /* MDC时钟 */
        ETH_PHY_IO_Init();

        /* 设置PHY地址为32 */
        pobj->devaddr = ETH_CHIP_MAX_DEV_ADDR + 1;

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
                pobj->devaddr = addr;
                status = ETH_CHIP_STATUS_OK;
                break;
            }
        }

        /* 判断这个PHY地址是否大于32（2^5）*/
        if (pobj->devaddr > ETH_CHIP_MAX_DEV_ADDR)
        {
            status = ETH_CHIP_STATUS_ADDRESS_ERROR;
        }

        /* 如果PHY地址有效 */
        if (status == ETH_CHIP_STATUS_OK)
        {
            /* 设置软件复位  */
            if (ETH_PHY_IO_WriteReg(pobj->devaddr, ETH_CHIP_BCR, ETH_CHIP_BCR_SOFT_RESET) >= 0)
            {
                /* 获取软件重置状态 */
                if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &regvalue) >= 0)
                {
                    tickstart = ETH_PHY_IO_GetTick();

                    /* 等待软件复位完成或超时  */
                    while (regvalue & ETH_CHIP_BCR_SOFT_RESET)
                    {
                        if ((ETH_PHY_IO_GetTick() - tickstart) <= ETH_CHIP_SW_RESET_TO)
                        {
                            if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &regvalue) < 0)
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
        pobj->is_initialized = 1;
    }

    return status;
}

/**
  * @brief       反初始化ETH_CHIP及其硬件资源
  * @param       pobj: 设备对象
  * @retval      ETH_CHIP_STATUS_OK：反初始化失败成功
                 ETH_CHIP_STATUS_ERROR：反初始化失败
  */
int32_t eth_chip_deinit(eth_chip_object_t *pobj)
{
    if (pobj->is_initialized)
    {
        if (ETH_PHY_IO_DeInit() < 0)
        {
            return ETH_CHIP_STATUS_ERROR;
        }

        pobj->is_initialized = 0;
    }

    return ETH_CHIP_STATUS_OK;
}

/**
  * @brief       关闭ETH_CHIP的下电模式
  * @param       pobj: 设备对象
  * @retval      ETH_CHIP_STATUS_OK：关闭成功
                 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
                 ETH_CHIP_STATUS_WRITE_ERROR：不能写寄存器
  */
int32_t eth_chip_disable_power_down_mode(eth_chip_object_t *pobj)
{
    uint32_t readval = 0;
    int32_t status = ETH_CHIP_STATUS_OK;

    if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &readval) >= 0)
    {
        readval &= ~ETH_CHIP_BCR_POWER_DOWN;

        /* 清除下电模式 */
        if (ETH_PHY_IO_WriteReg(pobj->devaddr, ETH_CHIP_BCR, readval) < 0)
        {
            status = ETH_CHIP_STATUS_WRITE_ERROR;
        }
    }
    else
    {
        status = ETH_CHIP_STATUS_READ_ERROR;
    }

    return status;
}

/**
  * @brief       使能ETH_CHIP的下电模式
  * @param       pobj: 设备对象
  * @retval      ETH_CHIP_STATUS_OK：关闭成功
                 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
                 ETH_CHIP_STATUS_WRITE_ERROR：不能写寄存器
  */
int32_t eth_chip_enable_power_down_mode(eth_chip_object_t *pobj)
{
    uint32_t readval = 0;
    int32_t status = ETH_CHIP_STATUS_OK;

    if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &readval) >= 0)
    {
        readval |= ETH_CHIP_BCR_POWER_DOWN;

        /* 使能下电模式 */
        if (ETH_PHY_IO_WriteReg(pobj->devaddr, ETH_CHIP_BCR, readval) < 0)
        {
            status = ETH_CHIP_STATUS_WRITE_ERROR;
        }
    }
    else
    {
        status = ETH_CHIP_STATUS_READ_ERROR;
    }

    return status;
}

/**
  * @brief       启动自动协商过程
  * @param       pobj: 设备对象
  * @retval      ETH_CHIP_STATUS_OK：关闭成功
                 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
                 ETH_CHIP_STATUS_WRITE_ERROR：不能写寄存器
  */
int32_t eth_chip_start_auto_nego(eth_chip_object_t *pobj)
{
    uint32_t readval = 0;
    int32_t status = ETH_CHIP_STATUS_OK;

    if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &readval) >= 0)
    {
        readval |= ETH_CHIP_BCR_AUTONEGO_EN;

        /* 启动自动协商 */
        if (ETH_PHY_IO_WriteReg(pobj->devaddr, ETH_CHIP_BCR, readval) < 0)
        {
            status = ETH_CHIP_STATUS_WRITE_ERROR;
        }
    }
    else
    {
        status = ETH_CHIP_STATUS_READ_ERROR;
    }

    return status;
}

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
int32_t eth_chip_get_link_state(eth_chip_object_t *pobj)
{
    uint32_t readval = 0;

    /* 检测特殊功能寄存器链接值 */
    if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_PHYSCSR, &readval) < 0)
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
int32_t eth_chip_set_link_state(eth_chip_object_t *pobj, uint32_t linkstate)
{
    uint32_t bcrvalue = 0;
    int32_t status = ETH_CHIP_STATUS_OK;

    if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &bcrvalue) >= 0)
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
        if (ETH_PHY_IO_WriteReg(pobj->devaddr, ETH_CHIP_BCR, bcrvalue) < 0)
        {
            status = ETH_CHIP_STATUS_WRITE_ERROR;
        }
    }

    return status;
}

/**
  * @brief       启用环回模式
  * @param       pobj: 设备对象
  * @param       pLinkState: 指向链路状态的指针
  * @retval      ETH_CHIP_STATUS_OK：设置成功
                 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
                 ETH_CHIP_STATUS_WRITE_ERROR ：不能写入寄存器
  */
int32_t eth_chip_enable_loop_back_mode(eth_chip_object_t *pobj)
{
    uint32_t readval = 0;
    int32_t status = ETH_CHIP_STATUS_OK;

    if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &readval) >= 0)
    {
        readval |= ETH_CHIP_BCR_LOOPBACK;

        /* 启用环回模式 */
        if (ETH_PHY_IO_WriteReg(pobj->devaddr, ETH_CHIP_BCR, readval) < 0)
        {
            status = ETH_CHIP_STATUS_WRITE_ERROR;
        }
    }
    else
    {
        status = ETH_CHIP_STATUS_READ_ERROR;
    }

    return status;
}

/**
  * @brief       禁用环回模式
  * @param       pobj: 设备对象
  * @param       pLinkState: 指向链路状态的指针
  * @retval      ETH_CHIP_STATUS_OK：设置成功
                 ETH_CHIP_STATUS_READ_ERROR：不能读取寄存器
                 ETH_CHIP_STATUS_WRITE_ERROR ：不能写入寄存器
  */
int32_t eth_chip_disable_loop_back_mode(eth_chip_object_t *pobj)
{
    uint32_t readval = 0;
    int32_t status = ETH_CHIP_STATUS_OK;

    if (ETH_PHY_IO_ReadReg(pobj->devaddr, ETH_CHIP_BCR, &readval) >= 0)
    {
        readval &= ~ETH_CHIP_BCR_LOOPBACK;

        /* 禁用环回模式 */
        if (ETH_PHY_IO_WriteReg(pobj->devaddr, ETH_CHIP_BCR, readval) < 0)
        {
            status = ETH_CHIP_STATUS_WRITE_ERROR;
        }
    }
    else
    {
        status = ETH_CHIP_STATUS_READ_ERROR;
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
        speed = ~((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS));
    }
    else if (PHY_TYPE == SR8201F)
    {
        DI_Console().WriteLine("SR8201F");

        /* 从SR8201F的0号寄存器中读取网络速度和双工模式 */
        speed = ((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 13);
    }
    else if (PHY_TYPE == YT8512C)
    {
        DI_Console().WriteLine("YT8512C");

        /* 从YT8512C的17号寄存器中读取网络速度和双工模式 */
        speed = ((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 14);
    }
    else if (PHY_TYPE == RTL8201)
    {
        DI_Console().WriteLine("RTL8201");

        /* 从RTL8201的16号寄存器中读取网络速度和双工模式 */
        speed = ((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 1);
    }

    return speed;
}
