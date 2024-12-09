#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H

#include <stdint.h>

#define LWIP_MAX_DHCP_TRIES 4 // DHCP服务器最大重试次数

// lwip控制结构体
typedef struct
{
    uint8_t mac[6];      // MAC地址
    uint8_t remoteip[4]; // 远端主机IP地址
    uint8_t ip[4];       // 本机IP地址
    uint8_t netmask[4];  // 子网掩码
    uint8_t gateway[4];  // 默认网关的IP地址

    uint8_t volatile dhcpstatus; // dhcp状态
                                 // 0,未获取DHCP地址;
                                 // 1,进入DHCP获取状态
                                 // 2,成功获取DHCP地址
                                 // 0XFF,获取失败.
} __lwip_dev;

extern __lwip_dev lwipdev; // lwip控制结构体

void lwip_pkt_handle(void);
void lwip_periodic_handle(void);

void lwip_comm_default_ip_set(__lwip_dev *lwipx);
uint8_t lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
uint8_t lwip_comm_init(void);
void lwip_dhcp_process_handle(void);

#endif
