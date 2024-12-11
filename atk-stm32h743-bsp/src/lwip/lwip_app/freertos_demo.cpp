/**
 ****************************************************************************************************
 * @file        freertos_demo.c
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

#include "FreeRTOS.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "task.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/interrupt.h>
#include <lwip_demo.h>

/******************************************************************************************************/
/*FreeRTOS����*/

/* LWIP_DEMO ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LWIP_DMEO_TASK_PRIO 11           /* �������ȼ� */
#define LWIP_DMEO_STK_SIZE 1024          /* �����ջ��С */
TaskHandle_t LWIP_Task_Handler;          /* ������ */
void lwip_demo_task(void *pvParameters); /* ������ */

/* KEY_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define KEY_TASK_PRIO 13           /* �������ȼ� */
#define KEY_STK_SIZE 1024          /* �����ջ��С */
TaskHandle_t KEYTask_Handler;      /* ������ */
void key_task(void *pvParameters); /* ������ */

/* DISPLAY_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define DISPLAY_TASK_PRIO 7            /* �������ȼ� */
#define DISPLAY_STK_SIZE 1024          /* �����ջ��С */
TaskHandle_t DISPLAYTask_Handler;      /* ������ */
void display_task(void *pvParameters); /* ������ */

/* ��ʾ��Ϣ���е����� */
#define DISPLAYMSG_Q_NUM 20    /* ��ʾ��Ϣ���е����� */
QueueHandle_t g_display_queue; /* ��ʾ��Ϣ���о�� */

/******************************************************************************************************/

/**
 * @breif       freertos_demo
 * @param       ��
 * @retval      ��
 */
void freertos_demo()
{
    while (lwip_comm_init() != 0)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{500});
    }

    DI_Console().WriteLine("lwip_comm_init successfully");

    while (g_lwipdev.dhcpstatus != 2 && g_lwipdev.dhcpstatus != 0xff) /* �ȴ���̬�Ͷ�̬�������  */
    {
        DI_Delayer().Delay(std::chrono::milliseconds{500});
    }

    DI_DoGlobalCriticalWork([&]()
                            {
                                g_display_queue = xQueueCreate(DISPLAYMSG_Q_NUM, 200); /* ������ϢMessage_Queue,���������200���� */

                                /* ����lwIP���� */
                                xTaskCreate((TaskFunction_t)lwip_demo_task,
                                            (char const *)"lwip_demo_task",
                                            (uint16_t)LWIP_DMEO_STK_SIZE,
                                            (void *)NULL,
                                            (UBaseType_t)LWIP_DMEO_TASK_PRIO,
                                            (TaskHandle_t *)&LWIP_Task_Handler);

                                /* key���� */
                                xTaskCreate((TaskFunction_t)key_task,
                                            (char const *)"key_task",
                                            (uint16_t)KEY_STK_SIZE,
                                            (void *)NULL,
                                            (UBaseType_t)KEY_TASK_PRIO,
                                            (TaskHandle_t *)&KEYTask_Handler);

                                /* ��ʾ���� */
                                xTaskCreate((TaskFunction_t)display_task,
                                            (char const *)"display_task",
                                            (uint16_t)DISPLAY_STK_SIZE,
                                            (void *)NULL,
                                            (UBaseType_t)DISPLAY_TASK_PRIO,
                                            (TaskHandle_t *)&DISPLAYTask_Handler);
                            });
}

/**
 * @brief       lwIP��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lwip_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;

    lwip_demo(); /* lwip���Դ��� */

    while (1)
    {
        vTaskDelay(5);
    }
}

/**
 * @brief       key_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void key_task(void *pvParameters)
{
    pvParameters = pvParameters;
    while (1)
    {
        g_lwip_send_flag |= LWIP_SEND_DATA; /* ���LWIP������Ҫ���� */
        vTaskDelay(10000);
    }
}

/**
 * @brief       ��ʾ����
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void display_task(void *pvParameters)
{
    pvParameters = pvParameters;
    uint8_t buffer[200];

    while (1)
    {
        if (g_display_queue != NULL)
        {
            if (xQueueReceive(g_display_queue, buffer, portMAX_DELAY))
            {
            }
        }

        vTaskDelay(5);
    }
}
