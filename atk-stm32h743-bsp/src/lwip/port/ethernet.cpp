#include "ethernet.h"
#include "ethernet_chip.h"
#include "lwip_comm.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>

/* 以太网句柄 */
ETH_HandleTypeDef g_eth_handler;

/**
 * @brief       以太网芯片初始化
 * @param       无
 * @retval      0,成功
 *              1,失败
 */
uint8_t ethernet_init(void)
{
    base::Mac mac{
        std::endian::big,
        base::Array<uint8_t, 6>{
            0xB8,
            0xAE,
            0x1D,
            0x00,
            0x04,
            0x00,
        },
    };

    DI_EthernetController().Open(bsp::IEthernetController_InterfaceType::RMII, mac);

    uint8_t macaddress[6];
    macaddress[0] = g_lwipdev.mac[0];
    macaddress[1] = g_lwipdev.mac[1];
    macaddress[2] = g_lwipdev.mac[2];
    macaddress[3] = g_lwipdev.mac[3];
    macaddress[4] = g_lwipdev.mac[4];
    macaddress[5] = g_lwipdev.mac[5];

    g_eth_handler.Instance = ETH;
    g_eth_handler.Init.MACAddr = macaddress;
    g_eth_handler.Init.MediaInterface = HAL_ETH_RMII_MODE;
    g_eth_handler.Init.RxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(0x30040000);
    g_eth_handler.Init.TxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(0x30040000 + 4 * sizeof(ETH_DMADescTypeDef));
    g_eth_handler.Init.RxBuffLen = ETH_MAX_PACKET_SIZE;
    HAL_StatusTypeDef result = HAL_ETH_Init(&g_eth_handler);
    if (result != HAL_OK)
    {
        throw std::runtime_error{"初始化网络接口失败"};
    }

    return 0;
}

/**
 * @brief       ETH底层驱动，时钟使能，引脚配置
 *    @note     此函数会被HAL_ETH_Init()调用
 * @param       heth:以太网句柄
 * @retval      无
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    uint32_t regval;

    /* 关闭所有中断，复位过程不能被打断！ */
    DI_DoGlobalCriticalWork(
        [&]()
        {
            /* 判断开发板是否是旧版本(老板卡板载的是LAN8720A，而新板卡板载的是YT8512C) */
            regval = ethernet_read_phy(2);
            if (regval && 0xFFF == 0xFFF) /* 旧板卡（LAN8720A）引脚复位 */
            {
                DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1); /* 硬件复位 */
                DI_Delayer().Delay(std::chrono::milliseconds{100});
                DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0); /* 复位结束 */
                DI_Delayer().Delay(std::chrono::milliseconds{100});
            }
            else /* 新板卡（YT8512C）引脚复位 */
            {
                DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0); /* 硬件复位 */
                DI_Delayer().Delay(std::chrono::milliseconds{100});
                DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1); /* 复位结束 */
                DI_Delayer().Delay(std::chrono::milliseconds{100});
            }
        });

    /* 这里的中断优先级必须设置在 freertos 能够屏蔽的优先级范围内，不然不知道什么原因，
     * 会导致 freertos 的 queue.c 中报错。
     */
    DI_EnableInterrupt(static_cast<uint32_t>(ETH_IRQn), 7);
    DI_IsrManager().AddIsr(static_cast<uint32_t>(ETH_IRQn),
                           []()
                           {
                               HAL_ETH_IRQHandler(&g_eth_handler);
                           });
}

/**
 * @breif       读取以太网芯片寄存器值
 * @param       reg：读取的寄存器地址
 * @retval      regval：返回读取的寄存器值
 */
uint32_t ethernet_read_phy(uint16_t reg)
{
    uint32_t regval;
    HAL_ETH_ReadPHYRegister(&g_eth_handler, ETH_CHIP_ADDR, reg, &regval);
    return regval;
}

/**
 * @breif       向以太网芯片指定地址写入寄存器值
 * @param       reg   : 要写入的寄存器
 * @param       value : 要写入的寄存器
 * @retval      无
 */
void ethernet_write_phy(uint16_t reg, uint16_t value)
{
    uint32_t temp = value;
    HAL_ETH_WritePHYRegister(&g_eth_handler, ETH_CHIP_ADDR, reg, temp);
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
        speed = ~((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS));
    }
    else if (PHY_TYPE == SR8201F)
    {
        DI_Console().WriteLine("SR8201F");

        /* 从SR8201F的0号寄存器中读取网络速度和双工模式 */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 13);
    }
    else if (PHY_TYPE == YT8512C)
    {
        DI_Console().WriteLine("YT8512C");

        /* 从YT8512C的17号寄存器中读取网络速度和双工模式 */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 14);
    }
    else if (PHY_TYPE == RTL8201)
    {
        DI_Console().WriteLine("RTL8201");

        /* 从RTL8201的16号寄存器中读取网络速度和双工模式 */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 1);
    }

    return speed;
}
