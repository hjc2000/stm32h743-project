#include "ethernet.h"
#include "ethernet_chip.h"
#include "lwip_comm.h"
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>

ETH_HandleTypeDef g_eth_handler; /* ��̫����� */

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
 * @brief       ��̫��оƬ��ʼ��
 * @param       ��
 * @retval      0,�ɹ�
 *              1,ʧ��
 */
uint8_t ethernet_init(void)
{
    NETMPU_Config();

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
    g_eth_handler.Init.TxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(0x30040000 + sizeof(ETH_DMADescTypeDef) * 4);
    g_eth_handler.Init.RxBuffLen = ETH_MAX_PACKET_SIZE;

    if (HAL_ETH_Init(&g_eth_handler) == HAL_OK)
    {
        return 0; /* �ɹ� */
    }
    else
    {
        return 1; /* ʧ�� */
    }
}

/**
 * @brief       ETH�ײ�������ʱ��ʹ�ܣ���������
 *    @note     �˺����ᱻHAL_ETH_Init()����
 * @param       heth:��̫�����
 * @retval      ��
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE(); /* ����ETH_CLKʱ�� */
    __HAL_RCC_GPIOA_CLK_ENABLE(); /* ����ETH_MDIOʱ�� */
    __HAL_RCC_GPIOA_CLK_ENABLE(); /* ����ETH_CRSʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE(); /* ����ETH_MDCʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE(); /* ����ETH_RXD0ʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE(); /* ����ETH_RXD1ʱ�� */
    __HAL_RCC_GPIOB_CLK_ENABLE(); /* ����ETH_TX_ENʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE(); /* ����ETH_TXD0ʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE(); /* ����ETH_TXD1ʱ�� */

    /* Enable Ethernet clocks */
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();

    /* ������������ RMII�ӿ�
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
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;             /* ���츴�� */
    gpio_init_struct.Pull = GPIO_NOPULL;                 /* ���������� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;       /* ���� */
    gpio_init_struct.Alternate = GPIO_AF11_ETH;          /* ����ΪETH���� */
    HAL_GPIO_Init(ETH_CLK_GPIO_PORT, &gpio_init_struct); /* ETH_CLK����ģʽ���� */

    gpio_init_struct.Pin = ETH_MDIO_GPIO_PIN;
    HAL_GPIO_Init(ETH_MDIO_GPIO_PORT, &gpio_init_struct); /* ETH_MDIO����ģʽ���� */

    gpio_init_struct.Pin = ETH_CRS_GPIO_PIN;
    HAL_GPIO_Init(ETH_CRS_GPIO_PORT, &gpio_init_struct); /* ETH_CRS����ģʽ���� */

    /* PC1 */
    gpio_init_struct.Pin = ETH_MDC_GPIO_PIN;
    HAL_GPIO_Init(ETH_MDC_GPIO_PORT, &gpio_init_struct); /* ETH_MDC��ʼ�� */

    /* PC4 */
    gpio_init_struct.Pin = ETH_RXD0_GPIO_PIN;
    HAL_GPIO_Init(ETH_RXD0_GPIO_PORT, &gpio_init_struct); /* ETH_RXD0��ʼ�� */

    /* PC5 */
    gpio_init_struct.Pin = ETH_RXD1_GPIO_PIN;
    HAL_GPIO_Init(ETH_RXD1_GPIO_PORT, &gpio_init_struct); /* ETH_RXD1��ʼ�� */

    /* PB11,PG13,PG14 */
    gpio_init_struct.Pin = ETH_TX_EN_GPIO_PIN;
    HAL_GPIO_Init(ETH_TX_EN_GPIO_PORT, &gpio_init_struct); /* ETH_TX_EN��ʼ�� */

    gpio_init_struct.Pin = ETH_TXD0_GPIO_PIN;
    HAL_GPIO_Init(ETH_TXD0_GPIO_PORT, &gpio_init_struct); /* ETH_TXD0��ʼ�� */

    gpio_init_struct.Pin = ETH_TXD1_GPIO_PIN;
    HAL_GPIO_Init(ETH_TXD1_GPIO_PORT, &gpio_init_struct); /* ETH_TXD1��ʼ�� */

    uint32_t regval;

    /* �ر������жϣ���λ���̲��ܱ���ϣ� */
    DI_DisableGlobalInterrupt();

    /* �жϿ������Ƿ��Ǿɰ汾(�ϰ忨���ص���LAN8720A�����°忨���ص���YT8512C) */
    regval = ethernet_read_phy(2);

    if (regval && 0xFFF == 0xFFF)
    {
        /* �ɰ忨��LAN8720A�����Ÿ�λ */
        /* Ӳ����λ */
        DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
        DI_Delayer().Delay(std::chrono::milliseconds{100});

        /* ��λ���� */
        DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
        DI_Delayer().Delay(std::chrono::milliseconds{100});
    }
    else
    {
        /* �°忨��YT8512C�����Ÿ�λ */
        /* Ӳ����λ */
        DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
        DI_Delayer().Delay(std::chrono::milliseconds{100});

        /* ��λ���� */
        DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
        DI_Delayer().Delay(std::chrono::milliseconds{100});
    }

    /* ���������ж� */
    DI_EnableGlobalInterrupt();
}

/**
 * @breif       ��ȡ��̫��оƬ�Ĵ���ֵ
 * @param       reg����ȡ�ļĴ�����ַ
 * @retval      regval�����ض�ȡ�ļĴ���ֵ
 */
uint32_t ethernet_read_phy(uint16_t reg)
{
    uint32_t regval;
    HAL_ETH_ReadPHYRegister(&g_eth_handler,
                            ETH_CHIP_ADDR,
                            reg,
                            &regval);

    return regval;
}

/**
 * @breif       ����̫��оƬָ����ַд��Ĵ���ֵ
 * @param       reg   : Ҫд��ļĴ���
 * @param       value : Ҫд��ļĴ���
 * @retval      ��
 */
void ethernet_write_phy(uint16_t reg, uint16_t value)
{
    uint32_t temp = value;
    HAL_ETH_WritePHYRegister(&g_eth_handler, ETH_CHIP_ADDR, reg, temp);
}

/**
 * @breif       �������оƬ���ٶ�ģʽ
 * @param       ��
 * @retval      1:100M
                0:10M
 */
uint8_t ethernet_chip_get_speed(void)
{
    uint8_t speed = 0;
    if (PHY_TYPE == LAN8720)
    {
        /* ��LAN8720��31�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ */
        speed = ~((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS));
    }
    else if (PHY_TYPE == SR8201F)
    {
        /* ��SR8201F��0�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 13);
    }
    else if (PHY_TYPE == YT8512C)
    {
        /* ��YT8512C��17�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 14);
    }
    else if (PHY_TYPE == RTL8201)
    {
        /* ��RTL8201��16�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ */
        speed = ((ethernet_read_phy(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 1);
    }

    return speed;
}