#pragma once
#include "lwip/err.h"
#include "lwip/netif.h"

/* ������ʼ������ */
err_t ethernetif_init(netif *netif);

/* ���ݰ����뺯�� */
void ethernetif_input(netif *netif);

extern "C"
{
    u32_t sys_now(void); /* �ṩlwIPʱ�� */
}
