#pragma once
#include "lwip/snmp.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"

/* DHCP进程状态 */
#define LWIP_DHCP_OFF (uint8_t)0              /* DHCP服务器关闭状态 */
#define LWIP_DHCP_START (uint8_t)1            /* DHCP服务器启动状态 */
#define LWIP_DHCP_WAIT_ADDRESS (uint8_t)2     /* DHCP服务器等待分配IP状态 */
#define LWIP_DHCP_ADDRESS_ASSIGNED (uint8_t)3 /* DHCP服务器地址已分配状态 */
#define LWIP_DHCP_TIMEOUT (uint8_t)4          /* DHCP服务器超时状态 */
#define LWIP_DHCP_LINK_DOWN (uint8_t)5        /* DHCP服务器链接失败状态 */

/* DHCP服务器最大重试次数 */
#define LWIP_MAX_DHCP_TRIES (uint8_t)4
