/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET UDP ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ H743������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "lwip_demo.h"
#include "FreeRTOS.h"
#include "lwip/api.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include <lwip/sockets.h>
#include <stdint.h>
#include <stdio.h>

/* ����Զ��IP��ַ */
// #define DEST_IP_ADDR0               192
// #define DEST_IP_ADDR1               168
// #define DEST_IP_ADDR2                 1
// #define DEST_IP_ADDR3               167

/* ��Ҫ�Լ�����Զ��IP��ַ */
#define IP_ADDR "192.168.1.111"

#define LWIP_DEMO_RX_BUFSIZE 200                     /* ���������ݳ��� */
#define LWIP_DEMO_PORT 8080                          /* ���ӵı��ض˿ں� */
#define LWIP_SEND_THREAD_PRIO (tskIDLE_PRIORITY + 3) /* ���������߳����ȼ� */

/* �������ݻ����� */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE];
/* ������������ */
char g_lwip_demo_sendbuf[] = "ALIENTEK UDP TEST\r\n";
/* ���ݷ��ͱ�־λ */
uint8_t g_lwip_send_flag;
struct sockaddr_in local_info; /* ����Socket��ַ��Ϣ�ṹ�� */
socklen_t sock_fd;             /* ����һ��Socket�ӿ� */
static void lwip_send_thread(void *arg);

extern QueueHandle_t g_display_queue; /* ��ʾ��Ϣ���о�� */

/**
 * @brief       ���������߳�
 * @param       ��
 * @retval      ��
 */
void lwip_data_send(void)
{
    sys_thread_new("lwip_send_thread", lwip_send_thread, NULL, 512, LWIP_SEND_THREAD_PRIO);
}

/**
 * @brief       lwip_demoʵ�����
 * @param       ��
 * @retval      ��
 */
void lwip_demo(void)
{
    BaseType_t lwip_err;
    lwip_data_send();                                   /* �������������߳� */
    memset(&local_info, 0, sizeof(struct sockaddr_in)); /* ����������ַ��� */
    local_info.sin_len = sizeof(local_info);
    local_info.sin_family = AF_INET;                /* IPv4��ַ */
    local_info.sin_port = htons(LWIP_DEMO_PORT);    /* ���ö˿ں� */
    local_info.sin_addr.s_addr = htons(INADDR_ANY); /* ���ñ���IP��ַ */

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0); /* ����һ���µ�socket���� */

    /* ������ */
    bind(sock_fd, (struct sockaddr *)&local_info, sizeof(struct sockaddr_in));

    while (1)
    {
        memset(g_lwip_demo_recvbuf, 0, sizeof(g_lwip_demo_recvbuf));
        recv(sock_fd, (void *)g_lwip_demo_recvbuf, sizeof(g_lwip_demo_recvbuf), 0);

        lwip_err = xQueueSend(g_display_queue, &g_lwip_demo_recvbuf, 0);

        if (lwip_err == errQUEUE_FULL)
        {
            printf("����Key_Queue���������ݷ���ʧ��!\r\n");
        }
    }
}

/**
 * @brief       ���������̺߳���
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lwip_send_thread(void *pvParameters)
{
    pvParameters = pvParameters;

    local_info.sin_addr.s_addr = inet_addr(IP_ADDR); /* ��Ҫ���͵�Զ��IP��ַ */

    while (1)
    {
        if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) /* ������Ҫ���� */
        {
            sendto(sock_fd,                        /* scoket */
                   (char *)g_lwip_demo_sendbuf,    /* ���͵����� */
                   sizeof(g_lwip_demo_sendbuf), 0, /* ���͵����ݴ�С */
                   (struct sockaddr *)&local_info, /* ���ն˵�ַ��Ϣ */
                   sizeof(local_info));            /* ���ն˵�ַ��Ϣ��С */

            g_lwip_send_flag &= ~LWIP_SEND_DATA;
        }

        vTaskDelay(100);
    }
}
