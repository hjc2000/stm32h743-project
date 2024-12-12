#include "ethernet.h"
#include "ethernet_chip.h"
#include "lwip_comm.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>

/* 以太网句柄 */
ETH_HandleTypeDef g_eth_handler;

/**
 * @brief  Configure the MPU attributes
 * @param  None
 * @retval None
 */
void NETMPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    HAL_MPU_Disable();
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x30040000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER5;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief       以太网芯片初始化
 * @param       无
 * @retval      0,成功
 *              1,失败
 */
uint8_t ethernet_init(void)
{
    uint8_t macaddress[6];

    NETMPU_Config();

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

    if (HAL_ETH_Init(&g_eth_handler) == HAL_OK)
    {
        return 0; /* 成功 */
    }
    else
    {
        return 1; /* 失败 */
    }
}

/**
 * @brief       ETH底层驱动，时钟使能，引脚配置
 *    @note     此函数会被HAL_ETH_Init()调用
 * @param       heth:以太网句柄
 * @retval      无
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef gpio_init_struct;

    ETH_CLK_GPIO_CLK_ENABLE();   /* 开启ETH_CLK时钟 */
    ETH_MDIO_GPIO_CLK_ENABLE();  /* 开启ETH_MDIO时钟 */
    ETH_CRS_GPIO_CLK_ENABLE();   /* 开启ETH_CRS时钟 */
    ETH_MDC_GPIO_CLK_ENABLE();   /* 开启ETH_MDC时钟 */
    ETH_RXD0_GPIO_CLK_ENABLE();  /* 开启ETH_RXD0时钟 */
    ETH_RXD1_GPIO_CLK_ENABLE();  /* 开启ETH_RXD1时钟 */
    ETH_TX_EN_GPIO_CLK_ENABLE(); /* 开启ETH_TX_EN时钟 */
    ETH_TXD0_GPIO_CLK_ENABLE();  /* 开启ETH_TXD0时钟 */
    ETH_TXD1_GPIO_CLK_ENABLE();  /* 开启ETH_TXD1时钟 */

    /* Enable Ethernet clocks */
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();

    /* 网络引脚设置 RMII接口
     * ETH_MDIO -------------------------> PA2
     * ETH_MDC --------------------------> PC1
     * ETH_RMII_REF_CLK------------------> PA1
     * ETH_RMII_CRS_DV ------------------> PA7
     * ETH_RMII_RXD0 --------------------> PC4
     * ETH_RMII_RXD1 --------------------> PC5
     * ETH_RMII_TX_EN -------------------> PB11
     * ETH_RMII_TXD0 --------------------> PG13
     * ETH_RMII_TXD1 --------------------> PG14
     */

    /* PA1,2,7 */
    gpio_init_struct.Pin = ETH_CLK_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;             /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_NOPULL;                 /* 不带上下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;       /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF11_ETH;          /* 复用为ETH功能 */
    HAL_GPIO_Init(ETH_CLK_GPIO_PORT, &gpio_init_struct); /* ETH_CLK引脚模式设置 */

    gpio_init_struct.Pin = ETH_MDIO_GPIO_PIN;
    HAL_GPIO_Init(ETH_MDIO_GPIO_PORT, &gpio_init_struct); /* ETH_MDIO引脚模式设置 */

    gpio_init_struct.Pin = ETH_CRS_GPIO_PIN;
    HAL_GPIO_Init(ETH_CRS_GPIO_PORT, &gpio_init_struct); /* ETH_CRS引脚模式设置 */

    /* PC1 */
    gpio_init_struct.Pin = ETH_MDC_GPIO_PIN;
    HAL_GPIO_Init(ETH_MDC_GPIO_PORT, &gpio_init_struct); /* ETH_MDC初始化 */

    /* PC4 */
    gpio_init_struct.Pin = ETH_RXD0_GPIO_PIN;
    HAL_GPIO_Init(ETH_RXD0_GPIO_PORT, &gpio_init_struct); /* ETH_RXD0初始化 */

    /* PC5 */
    gpio_init_struct.Pin = ETH_RXD1_GPIO_PIN;
    HAL_GPIO_Init(ETH_RXD1_GPIO_PORT, &gpio_init_struct); /* ETH_RXD1初始化 */

    /* PB11,PG13,PG14 */
    gpio_init_struct.Pin = ETH_TX_EN_GPIO_PIN;
    HAL_GPIO_Init(ETH_TX_EN_GPIO_PORT, &gpio_init_struct); /* ETH_TX_EN初始化 */

    gpio_init_struct.Pin = ETH_TXD0_GPIO_PIN;
    HAL_GPIO_Init(ETH_TXD0_GPIO_PORT, &gpio_init_struct); /* ETH_TXD0初始化 */

    gpio_init_struct.Pin = ETH_TXD1_GPIO_PIN;
    HAL_GPIO_Init(ETH_TXD1_GPIO_PORT, &gpio_init_struct); /* ETH_TXD1初始化 */

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
        /* 从LAN8720的31号寄存器中读取网络速度和双工模式 */
        speed = ~((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS));
    }
    else if (PHY_TYPE == SR8201F)
    {
        /* 从SR8201F的0号寄存器中读取网络速度和双工模式 */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 13);
    }
    else if (PHY_TYPE == YT8512C)
    {
        /* 从YT8512C的17号寄存器中读取网络速度和双工模式 */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 14);
    }
    else if (PHY_TYPE == RTL8201)
    {
        /* 从RTL8201的16号寄存器中读取网络速度和双工模式 */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 1);
    }

    return speed;
}
