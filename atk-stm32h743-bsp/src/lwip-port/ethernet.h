#pragma once
#include <hal.h>

#define ETH_CLK_GPIO_PORT GPIOA
#define ETH_CLK_GPIO_PIN GPIO_PIN_1

#define ETH_MDIO_GPIO_PORT GPIOA
#define ETH_MDIO_GPIO_PIN GPIO_PIN_2

#define ETH_CRS_GPIO_PORT GPIOA
#define ETH_CRS_GPIO_PIN GPIO_PIN_7

#define ETH_MDC_GPIO_PORT GPIOC
#define ETH_MDC_GPIO_PIN GPIO_PIN_1

#define ETH_RXD0_GPIO_PORT GPIOC
#define ETH_RXD0_GPIO_PIN GPIO_PIN_4

#define ETH_RXD1_GPIO_PORT GPIOC
#define ETH_RXD1_GPIO_PIN GPIO_PIN_5

#define ETH_TX_EN_GPIO_PORT GPIOB
#define ETH_TX_EN_GPIO_PIN GPIO_PIN_11

#define ETH_TXD0_GPIO_PORT GPIOG
#define ETH_TXD0_GPIO_PIN GPIO_PIN_13

#define ETH_TXD1_GPIO_PORT GPIOG
#define ETH_TXD1_GPIO_PIN GPIO_PIN_14

/******************************************************************************************/

uint8_t ethernet_init(void);                           /* ��̫��оƬ��ʼ�� */
uint32_t ethernet_read_phy(uint16_t reg);              /* ��ȡ��̫��оƬ�Ĵ���ֵ */
void ethernet_write_phy(uint16_t reg, uint16_t value); /* ����̫��оƬָ����ַд��Ĵ���ֵ */
uint8_t ethernet_chip_get_speed(void);                 /* �����̫��оƬ���ٶ�ģʽ */
void NETMPU_Config(void);
