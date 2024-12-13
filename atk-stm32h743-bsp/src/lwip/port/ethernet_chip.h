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

/* PHY芯片进程状态 */
#define ETH_CHIP_STATUS_READ_ERROR ((int32_t) - 5)
#define ETH_CHIP_STATUS_WRITE_ERROR ((int32_t) - 4)
#define ETH_CHIP_STATUS_ADDRESS_ERROR ((int32_t) - 3)
#define ETH_CHIP_STATUS_RESET_TIMEOUT ((int32_t) - 2)
#define ETH_CHIP_STATUS_ERROR ((int32_t) - 1)
#define ETH_CHIP_STATUS_OK ((int32_t)0)
#define ETH_CHIP_STATUS_LINK_DOWN ((int32_t)1)
#define ETH_CHIP_STATUS_100MBITS_FULLDUPLEX ((int32_t)2)
#define ETH_CHIP_STATUS_100MBITS_HALFDUPLEX ((int32_t)3)
#define ETH_CHIP_STATUS_10MBITS_FULLDUPLEX ((int32_t)4)
#define ETH_CHIP_STATUS_10MBITS_HALFDUPLEX ((int32_t)5)
#define ETH_CHIP_STATUS_AUTONEGO_NOTDONE ((int32_t)6)

/* PHY地址 ---- 由用户设置 */
#define ETH_CHIP_ADDR ((uint16_t)0x0000U)
/* PHY寄存器的数量 */
#define ETH_CHIP_PHY_COUNT ((uint16_t)0x001FU)

#define YT8512C_AND_RTL8201BL_PHYREGISTER2 0x0000
#define SR8201F_PHYREGISTER2 0x001C
#define LAN8720A_PHYREGISTER2 0x0007

#define ETH_CHIP_SW_RESET_TO ((uint32_t)500U) /* 软件复位等待时间 */
#define ETH_CHIP_INIT_TO ((uint32_t)2000U)    /* 初始化等待时间 */
#define ETH_CHIP_MAX_DEV_ADDR ((uint32_t)31U) /* PHY地址的最大值 */

	/* PHY自动识别状态 */
	enum PHY_AUTO
	{
		PHY_AUTO_SELECT_NABLE = 0,
		PHY_AUTO_SELECT_DISABLE
	};

/* 使能/禁用PHY自动选择功能 */
#define PHY_AUTO_SELECT PHY_AUTO_SELECT_NABLE
#define LAN8720 0
#define SR8201F 1
#define YT8512C 2
#define RTL8201 3

	int32_t eth_chip_get_link_state();                   /* 获取ETH_CHIP设备的链路状态 */
	int32_t eth_chip_set_link_state(uint32_t linkstate); /* 设置ETH_CHIP设备的链路状态 */

#ifdef __cplusplus
}
#endif
