/**
 ******************************************************************************
 * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/ethernetif.c
 * @author  MCD Application Team
 * @brief   This file implements Ethernet network interface drivers for lwIP
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ethernetif.h"
#include "ethernet_chip.h"
#include "FreeRTOS.h"
#include "lwip/opt.h"
#include "lwip/pbuf.h"
#include "lwip_comm.h"
#include "semphr.h"
#include "string.h"
#include "task.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/system_time.h>
#include <bsp-interface/di/task.h>
#include <EthernetController.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT (portMAX_DELAY)
/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE (512)
#define NETIF_IN_TASK_PRIORITY (2)

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define ETH_RX_BUFFER_SIZE (1536UL)

#define ETH_DMA_TRANSMIT_TIMEOUT (20U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*
@Note: This interface is implemented to operate in zero-copy mode only:
		- Rx buffers are allocated statically and passed directly to the LwIP stack,
		  they will return back to ETH DMA after been processed by the stack.
		- Tx Buffers will be allocated from LwIP stack memory heap,
		  then passed to ETH HAL driver.

@Notes:
  1.a. ETH DMA Rx descriptors must be contiguous, the default count is 4,
	   to customize it please redefine ETH_RX_DESC_CNT in stm32xxxx_hal_conf.h
  1.b. ETH DMA Tx descriptors must be contiguous, the default count is 4,
	   to customize it please redefine ETH_TX_DESC_CNT in stm32xxxx_hal_conf.h

  2.a. Rx Buffers number must be between ETH_RX_DESC_CNT and 2*ETH_RX_DESC_CNT
  2.b. Rx Buffers must have the same size: ETH_RX_BUFFER_SIZE, this value must
	   passed to ETH DMA in the init field (bsp::EthernetController::Instance().Handle().Init.RxBuffLen)
  2.c  The RX Ruffers addresses and sizes must be properly defined to be aligned
	   to L1-CACHE line size (32 bytes).
*/

ETH_TxPacketConfig TxConfig;
QueueHandle_t g_rx_semaphore = NULL; /* 定义一个TX信号量 */

/* Private function prototypes -----------------------------------------------*/
void ethernetif_input(void *argument);

// u32_t  sys_now(void);
void pbuf_free_custom(struct pbuf *p);

int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);

LWIP_MEMPOOL_DECLARE(RX_POOL, 10, sizeof(struct pbuf_custom), "Zero-copy RX PBUF pool");

uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE] __attribute__((section(".ARM.__at_0x30040200"))); /* Ethernet Receive Buffers */

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
					   LL Driver Interface ( LwIP stack --> ETH)
*******************************************************************************/
/**
 * @brief In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
	uint32_t idx = 0;
	uint32_t duplex = 0;
	uint32_t speed = 0;
	ETH_MACConfigTypeDef g_eth_macconfig_handler{};

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

	DI_EthernetPort().Open(mac);
	netif->hwaddr_len = ETHARP_HWADDR_LEN; /* 设置MAC地址长度,为6个字节 */

	/* 初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复 */
	base::Span netif_mac_buff_span{netif->hwaddr, 6};
	netif_mac_buff_span.CopyFrom(mac.AsReadOnlySpan());
	netif_mac_buff_span.Reverse();

	netif->mtu = ETH_MAX_PAYLOAD; /* 最大允许传输单元,允许该网卡广播和ARP功能 */

	/* 网卡状态信息标志位，是很重要的控制字段，它包括网卡功能使能、广播 */
	/* 使能、 ARP 使能等等重要控制位 */
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	for (idx = 0; idx < ETH_RX_DESC_CNT; idx++)
	{
		HAL_ETH_DescAssignMemory(&bsp::EthernetController::Instance().Handle(),
								 idx,
								 Rx_Buff[idx],
								 NULL);
	}

	/* Initialize the RX POOL */
	LWIP_MEMPOOL_INIT(RX_POOL);

	/* Set Tx packet config common parameters */
	memset(&TxConfig, 0, sizeof(ETH_TxPacketConfig));
	TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
	TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;

	/* create a binary semaphore used for informing ethernetif of frame reception */
	g_rx_semaphore = xSemaphoreCreateBinary();

	/* create the task that handles the ETH_MAC */
	DI_TaskManager().Create(
		[netif]()
		{
			ethernetif_input(netif);
		},
		512);

	/* 必须等待初始化 */
	DI_Delayer().Delay(std::chrono::milliseconds{2000});
	try
	{
		if (DI_EthernetPort().Speed() == static_cast<base::Bps>(base::Mbps{100}))
		{
			speed = ETH_SPEED_100M;
		}
		else
		{
			speed = ETH_SPEED_10M;
		}

		if (DI_EthernetPort().DuplexMode() == bsp::IEthernetPort_DuplexMode::FullDuplex)
		{
			duplex = ETH_FULLDUPLEX_MODE;
		}
		else
		{
			duplex = ETH_HALFDUPLEX_MODE;
		}
	}
	catch (...)
	{
		DI_Console().WriteLine("获取连接双工和速度时发生异常。");
		netif_set_link_down(netif);
		netif_set_down(netif);
	}

	/* 配置MAC */
	HAL_ETH_GetMACConfig(&bsp::EthernetController::Instance().Handle(), &g_eth_macconfig_handler);
	g_eth_macconfig_handler.DuplexMode = duplex;
	g_eth_macconfig_handler.Speed = speed;
	HAL_ETH_SetMACConfig(&bsp::EthernetController::Instance().Handle(), &g_eth_macconfig_handler);
	HAL_ETH_Start(&bsp::EthernetController::Instance().Handle());

	/* 开启虚拟网卡 */
	netif_set_up(netif);
	netif_set_link_up(netif);
	while (!DI_EthernetController().ReadPHYRegister(0x1F)) /* 检查MCU与PHY芯片是否通信成功 */
	{
		printf("MCU与PHY芯片通信失败，请检查电路或者源码！！！！\r\n");
	}
}

/**
 * @brief This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	uint32_t i = 0;
	struct pbuf *q;
	err_t errval = ERR_OK;
	ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT];

	memset(Txbuffer, 0, ETH_TX_DESC_CNT * sizeof(ETH_BufferTypeDef));

	for (q = p; q != NULL; q = q->next)
	{
		if (i >= ETH_TX_DESC_CNT)
		{
			return ERR_IF;
		}

		Txbuffer[i].buffer = reinterpret_cast<uint8_t *>(q->payload);
		Txbuffer[i].len = q->len;

		if (i > 0)
		{
			Txbuffer[i - 1].next = &Txbuffer[i];
		}

		if (q->next == NULL)
		{
			Txbuffer[i].next = NULL;
		}

		i++;
	}

	TxConfig.Length = p->tot_len;
	TxConfig.TxBuffer = Txbuffer;

	HAL_ETH_Transmit(&bsp::EthernetController::Instance().Handle(),
					 &TxConfig,
					 ETH_DMA_TRANSMIT_TIMEOUT);

	return errval;
}

/**
 * @brief Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
	struct pbuf *p = NULL;
	ETH_BufferTypeDef RxBuff[ETH_RX_DESC_CNT];
	uint32_t framelength = 0, i = 0;
	struct pbuf_custom *custom_pbuf;

	memset(RxBuff, 0, ETH_RX_DESC_CNT * sizeof(ETH_BufferTypeDef));

	for (i = 0; i < ETH_RX_DESC_CNT - 1; i++)
	{
		RxBuff[i].next = &RxBuff[i + 1];
	}

	if (HAL_ETH_GetRxDataBuffer(&bsp::EthernetController::Instance().Handle(), RxBuff) == HAL_OK)
	{
		HAL_ETH_GetRxDataLength(&bsp::EthernetController::Instance().Handle(), &framelength);

		/* Build Rx descriptor to be ready for next data reception */
		HAL_ETH_BuildRxDescriptors(&bsp::EthernetController::Instance().Handle());

		/* Invalidate data cache for ETH Rx Buffers */
		SCB_InvalidateDCache_by_Addr((uint32_t *)RxBuff->buffer, framelength);

		custom_pbuf = (struct pbuf_custom *)LWIP_MEMPOOL_ALLOC(RX_POOL);

		if (custom_pbuf != NULL)
		{
			custom_pbuf->custom_free_function = pbuf_free_custom;

			p = pbuf_alloced_custom(PBUF_RAW,
									framelength,
									PBUF_REF,
									custom_pbuf,
									RxBuff->buffer,
									framelength);
		}
	}

	return p;
}

/**
 * @brief This function is the ethernetif_input task, it is processed when a packet
 * is ready to be read from the interface. It uses the function low_level_input()
 * that should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(void *argument)
{
	pbuf *p = nullptr;
	netif *netif = reinterpret_cast<struct netif *>(argument);
	while (true)
	{
		if (xSemaphoreTake(g_rx_semaphore, TIME_WAITING_FOR_INPUT) == pdTRUE)
		{
			do
			{
				p = low_level_input(netif);
				if (p != nullptr)
				{
					if (netif->input(p, netif) != ERR_OK)
					{
						pbuf_free(p);
					}
				}

			} while (p != nullptr);
		}
	}
}

/**
 * @brief Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;
	netif->linkoutput = low_level_output;

	/* initialize the hardware */
	low_level_init(netif);
	return ERR_OK;
}

/**
 * @brief  Custom Rx pbuf free callback
 * @param  pbuf: pbuf to be freed
 * @retval None
 */
void pbuf_free_custom(struct pbuf *p)
{
	struct pbuf_custom *custom_pbuf = (struct pbuf_custom *)p;
	LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);
}

/**
 * @brief  Ethernet Rx Transfer completed callback
 * @param  heth: ETH handle
 * @retval None
 */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
	portBASE_TYPE taskWoken = pdFALSE;

	if (xSemaphoreGiveFromISR(g_rx_semaphore, &taskWoken) == pdTRUE)
	{
		portEND_SWITCHING_ISR(taskWoken);
	}
}

/**
 * @brief  Read a PHY register through the MDIO interface.
 * @param  DevAddr: PHY port address
 * @param  RegAddr: PHY register address
 * @param  pRegVal: pointer to hold the register value
 * @retval 0 if OK -1 if Error
 */
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal)
{
	if (HAL_ETH_ReadPHYRegister(&bsp::EthernetController::Instance().Handle(), DevAddr, RegAddr, pRegVal) != HAL_OK)
	{
		return -1;
	}

	return 0;
}

/**
 * @brief  Write a value to a PHY register through the MDIO interface.
 * @param  DevAddr: PHY port address
 * @param  RegAddr: PHY register address
 * @param  RegVal: Value to be written
 * @retval 0 if OK -1 if Error
 */
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal)
{
	if (HAL_ETH_WritePHYRegister(&bsp::EthernetController::Instance().Handle(), DevAddr, RegAddr, RegVal) != HAL_OK)
	{
		return -1;
	}

	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
