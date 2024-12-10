#pragma once
#include "lwip/err.h"
#include "lwip/netif.h"

/* 网卡初始化函数 */
err_t ethernetif_init(netif *netif);

/* 数据包输入函数 */
void ethernetif_input(netif *netif);

extern "C"
{
    u32_t sys_now(void); /* 提供lwIP时基 */
}
