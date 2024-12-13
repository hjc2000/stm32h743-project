#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* PHY芯片寄存器映射表 */
#define ETH_CHIP_BCR ((uint16_t)0x0000U)
#define ETH_CHIP_BSR ((uint16_t)0x0001U)
#define PHY_REGISTER2 ((uint16_t)0x0002U)
#define PHY_REGISTER3 ((uint16_t)0x0003U)

/* 操作SCR寄存器的值（一般不需要修改） */
#define ETH_CHIP_BCR_SOFT_RESET ((uint16_t)0x8000U)
#define ETH_CHIP_BCR_LOOPBACK ((uint16_t)0x4000U)
#define ETH_CHIP_BCR_SPEED_SELECT ((uint16_t)0x2000U)
#define ETH_CHIP_BCR_AUTONEGO_EN ((uint16_t)0x1000U)
#define ETH_CHIP_BCR_POWER_DOWN ((uint16_t)0x0800U)
#define ETH_CHIP_BCR_ISOLATE ((uint16_t)0x0400U)
#define ETH_CHIP_BCR_RESTART_AUTONEGO ((uint16_t)0x0200U)
#define ETH_CHIP_BCR_DUPLEX_MODE ((uint16_t)0x0100U)

/* 操作BSR寄存器的值（一般不需要修改） */
#define ETH_CHIP_BSR_100BASE_T4 ((uint16_t)0x8000U)
#define ETH_CHIP_BSR_100BASE_TX_FD ((uint16_t)0x4000U)
#define ETH_CHIP_BSR_100BASE_TX_HD ((uint16_t)0x2000U)
#define ETH_CHIP_BSR_10BASE_T_FD ((uint16_t)0x1000U)
#define ETH_CHIP_BSR_10BASE_T_HD ((uint16_t)0x0800U)
#define ETH_CHIP_BSR_100BASE_T2_FD ((uint16_t)0x0400U)
#define ETH_CHIP_BSR_100BASE_T2_HD ((uint16_t)0x0200U)
#define ETH_CHIP_BSR_EXTENDED_STATUS ((uint16_t)0x0100U)
#define ETH_CHIP_BSR_AUTONEGO_CPLT ((uint16_t)0x0020U)
#define ETH_CHIP_BSR_REMOTE_FAULT ((uint16_t)0x0010U)
#define ETH_CHIP_BSR_AUTONEGO_ABILITY ((uint16_t)0x0008U)
#define ETH_CHIP_BSR_LINK_STATUS ((uint16_t)0x0004U)
#define ETH_CHIP_BSR_JABBER_DETECT ((uint16_t)0x0002U)
#define ETH_CHIP_BSR_EXTENDED_CAP ((uint16_t)0x0001U)

#ifdef __cplusplus
}
#endif
