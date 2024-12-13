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

#if PHY_AUTO_SELECT

	/* 选择PHY芯片 ---- 由用户设置 */

#define PHY_TYPE YT8512C

#if (PHY_TYPE == LAN8720)
#define ETH_CHIP_PHYSCSR ((uint16_t)0x1F)         /*!< tranceiver status register */
#define ETH_CHIP_SPEED_STATUS ((uint16_t)0x0004)  /*!< configured information of speed: 100Mbit/s */
#define ETH_CHIP_DUPLEX_STATUS ((uint16_t)0x0010) /*!< configured information of duplex: full-duplex */
#elif (PHY_TYPE == SR8201F)
#define ETH_CHIP_PHYSCSR ((uint16_t)0x00)         /*!< tranceiver status register */
#define ETH_CHIP_SPEED_STATUS ((uint16_t)0x2020)  /*!< configured information of speed: 100Mbit/s */
#define ETH_CHIP_DUPLEX_STATUS ((uint16_t)0x0100) /*!< configured information of duplex: full-duplex */
#elif (PHY_TYPE == YT8512C)
#define ETH_CHIP_PHYSCSR ((uint16_t)0x11)         /*!< tranceiver status register */
#define ETH_CHIP_SPEED_STATUS ((uint16_t)0x4010)  /*!< configured information of speed: 100Mbit/s */
#define ETH_CHIP_DUPLEX_STATUS ((uint16_t)0x2000) /*!< configured information of duplex: full-duplex */
#elif (PHY_TYPE == RTL8201)
#define PHY_SR ((uint16_t)0x10)                   /*!< tranceiver status register */
#define ETH_CHIP_SPEED_STATUS ((uint16_t)0x0022)  /*!< configured information of speed: 100Mbit/s */
#define ETH_CHIP_DUPLEX_STATUS ((uint16_t)0x0004) /*!< configured information of duplex: full-duplex */
#endif                                            /* PHY_TYPE */
#else
extern int PHY_TYPE;
extern uint16_t ETH_CHIP_PHYSCSR;
extern uint16_t ETH_CHIP_SPEED_STATUS;
extern uint16_t ETH_CHIP_DUPLEX_STATUS;
#endif

	/* 注册到组件对象结构体 */
	typedef struct
	{
		uint32_t is_initialized; /* 描述该设备是否初始化 */
		void *pdata;             /* 传入的形参 */
	} eth_chip_object_t;

	int32_t eth_chip_deinit(eth_chip_object_t *pobj);                             /* 反初始化ETH_CHIP及其硬件资源 */
	int32_t eth_chip_disable_power_down_mode(eth_chip_object_t *pobj);            /* 关闭ETH_CHIP的下电模式 */
	int32_t eth_chip_enable_power_down_mode(eth_chip_object_t *pobj);             /* 使能ETH_CHIP的下电模式 */
	int32_t eth_chip_get_link_state(eth_chip_object_t *pobj);                     /* 获取ETH_CHIP设备的链路状态 */
	int32_t eth_chip_set_link_state(eth_chip_object_t *pobj, uint32_t linkstate); /* 设置ETH_CHIP设备的链路状态 */
	int32_t eth_chip_enable_loop_back_mode(eth_chip_object_t *pobj);              /* 启用环回模式 */
	int32_t eth_chip_disable_loop_back_mode(eth_chip_object_t *pobj);             /* 禁用环回模式 */

#ifdef __cplusplus
}
#endif
