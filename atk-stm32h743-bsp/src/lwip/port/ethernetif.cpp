#include "ethernetif.h"
#include "FreeRTOS.h"
#include "lwip/opt.h"
#include "lwip/pbuf.h"
#include "lwip_comm.h"
#include "semphr.h"
#include "string.h"
#include "task.h"
#include <base/container/List.h>
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

/* Private function prototypes -----------------------------------------------*/
void ethernetif_input(void *argument);

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
static void low_level_init(netif *net_interface)
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
		netif_set_link_down(net_interface);
		netif_set_down(net_interface);
	}

	/* 设置MAC地址长度,为6个字节 */
	net_interface->hwaddr_len = ETHARP_HWADDR_LEN;

	/* 初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复 */
	base::Span netif_mac_buff_span{net_interface->hwaddr, 6};
	netif_mac_buff_span.CopyFrom(mac.AsReadOnlySpan());
	netif_mac_buff_span.Reverse();

	net_interface->mtu = ETH_MAX_PAYLOAD; /* 最大允许传输单元,允许该网卡广播和ARP功能 */

	/* 网卡状态信息标志位，是很重要的控制字段，它包括网卡功能使能、广播 */
	/* 使能、 ARP 使能等等重要控制位 */
	net_interface->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	/* 开启虚拟网卡 */
	netif_set_up(net_interface);
	netif_set_link_up(net_interface);

	/* create the task that handles the ETH_MAC */
	DI_TaskManager().Create(
		[net_interface]()
		{
			ethernetif_input(net_interface);
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
static err_t low_level_output(netif *net_interface, pbuf *p)
{
	pbuf *current_pbuf;
	base::List<base::ReadOnlySpan> spans{};

	for (current_pbuf = p; current_pbuf != nullptr; current_pbuf = current_pbuf->next)
	{
		base::ReadOnlySpan span{
			reinterpret_cast<uint8_t *>(current_pbuf->payload),
			current_pbuf->len,
		};

		spans.Add(span);
	}

	try
	{
		DI_EthernetPort().Send(spans);
		return err_enum_t::ERR_OK;
	}
	catch (std::exception const &e)
	{
		return err_enum_t::ERR_BUF;
	}
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
	while (true)
	{
		base::IEnumerable<base::ReadOnlySpan> const &spans = DI_EthernetPort().Receive();
		base::List<base::ReadOnlySpan> received_span_list{};

		while (true)
		{
			ETH_BufferTypeDef rx_buffers[ETH_RX_DESC_CNT]{};
			for (uint32_t i = 0; i < ETH_RX_DESC_CNT - 1; i++)
			{
				rx_buffers[i].next = &rx_buffers[i + 1];
			}

			if (!HAL_ETH_IsRxDataAvailable(&bsp::EthernetController::Instance().Handle()))
			{
				DI_Console().WriteLine("no rx data avaliable");
				break;
			}

			if (HAL_ETH_GetRxDataBuffer(&bsp::EthernetController::Instance().Handle(), rx_buffers) != HAL_OK)
			{
				DI_Console().WriteLine("HAL_ETH_GetRxDataBuffer error");
				break;
			}

			HAL_ETH_BuildRxDescriptors(&bsp::EthernetController::Instance().Handle());
			for (ETH_BufferTypeDef buffer : rx_buffers)
			{
				if (buffer.buffer == nullptr)
				{
					break;
				}

				if (buffer.len == 0)
				{
					break;
				}

				SCB_InvalidateDCache_by_Addr(buffer.buffer, buffer.len);
				base::ReadOnlySpan span{buffer.buffer, static_cast<int32_t>(buffer.len)};
				received_span_list.Add(span);
				if (buffer.next == nullptr)
				{
					break;
				}
			}

			DI_Console().WriteLine("received_span_list.Count = " + std::to_string(received_span_list.Count()));
			for (base::ReadOnlySpan &span : received_span_list)
			{
				pbuf_custom *custom_pbuf = new pbuf_custom{};
				custom_pbuf->custom_free_function = [](pbuf *p)
				{
					delete reinterpret_cast<pbuf_custom *>(p);
				};

				pbuf *p = pbuf_alloced_custom(PBUF_RAW,
											  span.Size(),
											  PBUF_REF,
											  custom_pbuf,
											  const_cast<uint8_t *>(span.Buffer()),
											  span.Size());

				netif *net_interface = reinterpret_cast<netif *>(argument);
				if (net_interface->input(p, net_interface) != err_enum_t::ERR_OK)
				{
					pbuf_free(p);
				}
			}
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
	return err_enum_t::ERR_OK;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
