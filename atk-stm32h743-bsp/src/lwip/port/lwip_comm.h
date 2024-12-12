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

/* 链接状态 */
#define LWIP_LINK_OFF (uint8_t)0   /* 链接关闭状态 */
#define LWIP_LINK_ON (uint8_t)1    /* 链接开启状态 */
#define LWIP_LINK_AGAIN (uint8_t)2 /* 重复开启 */

/* DHCP服务器最大重试次数 */
#define LWIP_MAX_DHCP_TRIES (uint8_t)4

/*lwip控制结构体*/
typedef struct
{
    uint8_t remoteip[4]; /* 远端主机IP地址 */
    uint8_t ip[4];       /* 本机IP地址 */
    uint8_t netmask[4];  /* 子网掩码 */
    uint8_t gateway[4];  /* 默认网关的IP地址 */
    uint8_t dhcpstatus;  /* dhcp状态
                         0, 未获取DHCP地址
                         1, 进入DHCP获取状态
                         2, 成功获取DHCP地址
                         0XFF,获取失败 */
    uint8_t link_status; /* 连接状态 */
} __lwip_dev;

extern __lwip_dev g_lwipdev; /* lwip控制结构体 */

void lwip_comm_default_ip_set(__lwip_dev *lwipx); /* lwip 默认IP设置 */
uint8_t lwip_comm_init(void);                     /* LWIP初始化(LWIP启动的时候使用) */
