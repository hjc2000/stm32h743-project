#include "ethernetif.h"
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

ETH_TxPacketConfig TxConfig{};

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

QueueHandle_t g_rx_semaphore = NULL; /* 定义一个TX信号量 */

/* Private function prototypes -----------------------------------------------*/
void ethernetif_input(void *argument);

// u32_t  sys_now(void);
void pbuf_free_custom(struct pbuf *p);

LWIP_MEMPOOL_DECLARE(RX_POOL, 10, sizeof(struct pbuf_custom), "Zero-copy RX PBUF pool");

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

	try
	{
		DI_EthernetPort().Open(mac);
	}
	catch (std::exception const &e)
	{
		DI_Console().WriteLine(e.what());
		DI_Console().WriteLine("打开网口失败");
		netif_set_link_down(netif);
		netif_set_down(netif);
	}

	/* 设置MAC地址长度,为6个字节 */
	netif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* 初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复 */
	base::Span netif_mac_buff_span{netif->hwaddr, 6};
	netif_mac_buff_span.CopyFrom(mac.AsReadOnlySpan());
	netif_mac_buff_span.Reverse();

	netif->mtu = ETH_MAX_PAYLOAD; /* 最大允许传输单元,允许该网卡广播和ARP功能 */

	/* 网卡状态信息标志位，是很重要的控制字段，它包括网卡功能使能、广播 */
	/* 使能、 ARP 使能等等重要控制位 */
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	/* Initialize the RX POOL */
	LWIP_MEMPOOL_INIT(RX_POOL);

	/* Set Tx packet config common parameters */
	TxConfig = ETH_TxPacketConfig{};
	TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
	TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;

	/* create a binary semaphore used for informing ethernetif of frame reception */
	g_rx_semaphore = xSemaphoreCreateBinary();

	/* 开启虚拟网卡 */
	netif_set_up(netif);
	netif_set_link_up(netif);

	/* create the task that handles the ETH_MAC */
	DI_TaskManager().Create(
		[netif]()
		{
			ethernetif_input(netif);
		},
		512);
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
static err_t low_level_output(struct netif *netif, pbuf *p)
{
	uint32_t i = 0;
	pbuf *current_pbuf;
	err_t errval = ERR_OK;
	ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT]{};

	for (current_pbuf = p; current_pbuf != NULL; current_pbuf = current_pbuf->next)
	{
		if (i >= ETH_TX_DESC_CNT)
		{
			return ERR_IF;
		}

		Txbuffer[i].buffer = reinterpret_cast<uint8_t *>(current_pbuf->payload);
		Txbuffer[i].len = current_pbuf->len;

		if (i > 0)
		{
			Txbuffer[i - 1].next = &Txbuffer[i];
		}

		if (current_pbuf->next == NULL)
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
	ETH_BufferTypeDef RxBuff[ETH_RX_DESC_CNT]{};
	uint32_t framelength = 0, i = 0;
	struct pbuf_custom *custom_pbuf;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
