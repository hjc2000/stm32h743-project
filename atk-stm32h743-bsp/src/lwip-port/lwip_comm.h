#pragma once
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
struct __lwip_dev
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
};

extern __lwip_dev g_lwipdev; /* lwip���ƽṹ�� */

void lwip_periodic_handle(void);                  /* lwip_periodic_handle */
void lwip_comm_default_ip_set(__lwip_dev *lwipx); /* lwip Ĭ��IP���� */
uint8_t lwip_comm_init(void);                     /* LWIP��ʼ��(LWIP������ʱ��ʹ��) */
void lwip_pkt_handle(void);                       /* ѭ�������������뺯�� */
