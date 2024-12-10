/**
 ****************************************************************************************************
 * @file        lwip_comm.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP��������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ H743������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211202
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H
#include "ethernet.h"
#include "netif/etharp.h"

/* DHCP����״̬ */
#define LWIP_DHCP_OFF (uint8_t)0              /* DHCP�������ر�״̬ */
#define LWIP_DHCP_START (uint8_t)1            /* DHCP����������״̬ */
#define LWIP_DHCP_WAIT_ADDRESS (uint8_t)2     /* DHCP�������ȴ�����IP״̬ */
#define LWIP_DHCP_ADDRESS_ASSIGNED (uint8_t)3 /* DHCP��������ַ�ѷ���״̬ */
#define LWIP_DHCP_TIMEOUT (uint8_t)4          /* DHCP��������ʱ״̬ */
#define LWIP_DHCP_LINK_DOWN (uint8_t)5        /* DHCP����������ʧ��״̬ */

/* DHCP������������Դ��� */
#define LWIP_MAX_DHCP_TRIES (uint8_t)4

/*lwip���ƽṹ��*/
typedef struct
{
    uint8_t mac[6];      /* MAC��ַ */
    uint8_t remoteip[4]; /* Զ������IP��ַ */
    uint8_t ip[4];       /* ����IP��ַ */
    uint8_t netmask[4];  /* �������� */
    uint8_t gateway[4];  /* Ĭ�����ص�IP��ַ */
    uint8_t dhcpstatus;  /* dhcp״̬
                         0, δ��ȡDHCP��ַ
                         1, ����DHCP��ȡ״̬
                         2, �ɹ���ȡDHCP��ַ
                         0XFF,��ȡʧ�� */
} __lwip_dev;

extern __lwip_dev g_lwipdev; /* lwip���ƽṹ�� */

void lwip_periodic_handle(void);                  /* lwip_periodic_handle */
void lwip_comm_default_ip_set(__lwip_dev *lwipx); /* lwip Ĭ��IP���� */
uint8_t lwip_comm_init(void);                     /* LWIP��ʼ��(LWIP������ʱ��ʹ��) */
void lwip_pkt_handle(void);                       /* ѭ�������������뺯�� */
#endif