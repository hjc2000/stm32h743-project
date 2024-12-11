/**
 ******************************************************************************
 * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/lwipopts.h
 * @author  MCD Application Team
 * @brief   lwIP Options Configuration.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/* NO_SYS ��ʾ�޲���ϵͳģ��㣬�޲���ϵͳΪ1���в���ϵͳ����Ϊ0
   ע������������û���벻ͬ */
#define NO_SYS 0

/* ---------- �ڴ�ѡ�� ---------- */
/* �ڴ���룬���� 4 �ֽڶ���  */
#define MEM_ALIGNMENT 4

/* ���ڴ�Ĵ�С�������Ҫ����Ķ��ڴ棬��ô���ø�һ�� */
#define MEM_SIZE (20 * 1024)

/* MEMP_NUM_PBUF: �����ڴ�ص�����  */
#define MEMP_NUM_PBUF 15
/* MEMP_NUM_UDP_PCB: UDPЭ����ƿ������. */
#define MEMP_NUM_UDP_PCB 4
/* MEMP_NUM_TCP_PCB: TCP������. */
#define MEMP_NUM_TCP_PCB 4
/* MEMP_NUM_TCP_PCB_LISTEN: ����TCP������. */
#define MEMP_NUM_TCP_PCB_LISTEN 2
/* MEMP_NUM_TCP_SEG: ͬʱ�Ŷӵ�TCP��������. */
#define MEMP_NUM_TCP_SEG 120
/* MEMP_NUM_SYS_TIMEOUT: ��ʱģ��������. */
#define MEMP_NUM_SYS_TIMEOUT 6

/* ---------- Pbufѡ�� ---------- */
/* PBUF_POOL �ڴ����ÿ���ڴ���С */
#define PBUF_POOL_SIZE 20
/* PBUF_POOL_BUFSIZE: pbuf����ÿ��pbuf�Ĵ�С. */
#define PBUF_POOL_BUFSIZE LWIP_MEM_ALIGN_SIZE(TCP_MSS + 40 + PBUF_LINK_ENCAPSULATION_HLEN + PBUF_LINK_HLEN)

/* ---------- TCPѡ�� ---------- */
#define LWIP_TCP 1
#define TCP_TTL 255

/* ����TCP�Ƿ�Ӧ�öԵ���Ķν����Ŷ�
   �����������豸�ڴ治�㣬����Ϊ0. */
#define TCP_QUEUE_OOSEQ 0

/* TCP���δ�С */
#define TCP_MSS (1500 - 40) /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP�����߻������ռ�(�ֽ�). */
#define TCP_SND_BUF (11 * TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP���ͻ������ռ䡣�����������
    ��Ҫ(2 * TCP_SND_BUF/TCP_MSS)������������ */

#define TCP_SND_QUEUELEN (8 * TCP_SND_BUF / TCP_MSS)

/* TCP���մ��� */
#define TCP_WND (2 * TCP_MSS)

/* ---------- ICMP ѡ�� ---------- */
#define LWIP_ICMP 1

/* ---------- DHCP ѡ�� ---------- */
/* �����ϣ��DHCP����Ϊ���뽫LWIP_DHCP����Ϊ1 */
#define LWIP_DHCP 1

/* ---------- UDP ѡ�� ---------- */
#define LWIP_UDP 1
#define UDP_TTL 255

/* ---------- Statistics ѡ�� ---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1

/* ---------- ���ӻص�ѡ�� ---------- */
/* WIP_NETIF_LINK_CALLBACK==1:֧�����ԽӿڵĻص�����
   ÿ�����Ӹı�(���磬��������)
 */
#define LWIP_NETIF_LINK_CALLBACK 1
/*
   --------------------------------------
   ---------- ֡У���ѡ�� ----------
   --------------------------------------
*/

/*
The STM32F4x7 allows computing and verifying the IP, UDP, TCP and ICMP checksums by hardware:
 - To use this feature let the following define uncommented.
 - To disable it and process by CPU comment the  the checksum.
*/
#define CHECKSUM_BY_HARDWARE

#ifdef CHECKSUM_BY_HARDWARE
/* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
#define CHECKSUM_GEN_IP 0
/* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP 0
/* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP 0
/* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
#define CHECKSUM_CHECK_IP 0
/* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP 0
/* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP 0
/* CHECKSUM_CHECK_ICMP==0: Check checksums by hardware for incoming ICMP packets.*/
#define CHECKSUM_GEN_ICMP 0
#else
/* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
#define CHECKSUM_GEN_IP 1
/* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
#define CHECKSUM_GEN_UDP 1
/* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
#define CHECKSUM_GEN_TCP 1
/* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
#define CHECKSUM_CHECK_IP 1
/* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
#define CHECKSUM_CHECK_UDP 1
/* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
#define CHECKSUM_CHECK_TCP 1
/* CHECKSUM_CHECK_ICMP==1: Check checksums by hardware for incoming ICMP packets.*/
#define CHECKSUM_GEN_ICMP 1
#endif

/*
   ----------------------------------------------
   ---------- �������ѡ�� ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1:����Netconn API(��Ҫʹ��api_lib.c)
 */
#define LWIP_NETCONN 1

/*
   ------------------------------------
   ---------- Socketѡ�� ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1:����Socket API(Ҫ��ʹ��Socket .c)
 */
#define LWIP_SOCKET 1

/*
   ------------------------------------
   ---------- httpd options ----------
   ------------------------------------
*/
/** Set this to 1 to include "fsdata_custom.c" instead of "fsdata.c" for the
 * file system (to prevent changing the file included in CVS) */
#define HTTPD_USE_CUSTOM_FSDATA 1
/*
   ---------------------------------
   ---------- ����ϵͳѡ�� ----------
   ---------------------------------
*/

#define TCPIP_THREAD_NAME "TCP/IP"
#define TCPIP_THREAD_STACKSIZE 1000
#define TCPIP_MBOX_SIZE 6
#define DEFAULT_UDP_RECVMBOX_SIZE 6
#define DEFAULT_TCP_RECVMBOX_SIZE 6
#define DEFAULT_ACCEPTMBOX_SIZE 6
#define DEFAULT_THREAD_STACKSIZE 500
#define TCPIP_THREAD_PRIO 5
#define LWIP_SO_RCVTIMEO 1

#endif /* __LWIPOPTS_H__ */