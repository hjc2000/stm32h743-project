/**
 ****************************************************************************************************
 * @file        freertos_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET TCPClient ʵ��
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

#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "lwip_comm.h"
#include "lwip_demo.h"
#include "lwipopts.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "task.h"
#include <bsp-interface/di/delayer.h>

/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 5            /* �������ȼ� */
#define START_STK_SIZE 512           /* �����ջ��С */
TaskHandle_t StartTask_Handler;      /* ������ */
void start_task(void *pvParameters); /* ������ */

/* LWIP_DEMO ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LWIP_DMEO_TASK_PRIO 11           /* �������ȼ� */
#define LWIP_DMEO_STK_SIZE 1024          /* �����ջ��С */
TaskHandle_t LWIP_Task_Handler;          /* ������ */
void lwip_demo_task(void *pvParameters); /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO 10           /* �������ȼ� */
#define LED_STK_SIZE 128           /* �����ջ��С */
TaskHandle_t LEDTask_Handler;      /* ������ */
void led_task(void *pvParameters); /* ������ */

/* KEY_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define KEY_TASK_PRIO 13           /* �������ȼ� */
#define KEY_STK_SIZE 128           /* �����ջ��С */
TaskHandle_t KEYTask_Handler;      /* ������ */
void key_task(void *pvParameters); /* ������ */

/* DISPLAY_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define DISPLAY_TASK_PRIO 7            /* �������ȼ� */
#define DISPLAY_STK_SIZE 512           /* �����ջ��С */
TaskHandle_t DISPLAYTask_Handler;      /* ������ */
void display_task(void *pvParameters); /* ������ */

/* ��ʾ��Ϣ���е����� */
#define DISPLAYMSG_Q_NUM 20    /* ��ʾ��Ϣ���е����� */
QueueHandle_t g_display_queue; /* ��ʾ��Ϣ���о�� */

/******************************************************************************************************/

/**
 * @breif       ����UI
 * @param       mode :  bit0:0,������;1,����ǰ�벿��UI
 *                      bit1:0,������;1,���غ�벿��UI
 * @retval      ��
 */
void lwip_test_ui(uint8_t mode)
{
    uint8_t speed;
    uint8_t buf[30];

    if (mode & 1 << 0)
    {
        // lcd_show_string(6, 10, 200, 32, 32, "STM32", DARKBLUE);
        // lcd_show_string(6, 40, lcddev.width, 24, 24, "lwIP TCPClient Test", DARKBLUE);
        // lcd_show_string(6, 70, 200, 16, 16, "ATOM@ALIENTEK", DARKBLUE);
    }

    if (mode & 1 << 1)
    {
        // lcd_show_string(5, 110, 200, 16, 16, "lwIP Init Successed", MAGENTA);

        if (g_lwipdev.dhcpstatus == 2)
        {
            sprintf((char *)buf, "DHCP IP:%d.%d.%d.%d", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]); /* ��ʾ��̬IP��ַ */
        }
        else
        {
            sprintf((char *)buf, "Static IP:%d.%d.%d.%d", g_lwipdev.ip[0], g_lwipdev.ip[1], g_lwipdev.ip[2], g_lwipdev.ip[3]); /* ��ӡ��̬IP��ַ */
        }

        // lcd_show_string(5, 130, 200, 16, 16, (char *)buf, MAGENTA);

        speed = ethernet_chip_get_speed(); /* �õ����� */

        if (speed)
        {
            // lcd_show_string(5, 150, 200, 16, 16, "Ethernet Speed:100M", MAGENTA);
        }
        else
        {
            // lcd_show_string(5, 150, 200, 16, 16, "Ethernet Speed:10M", MAGENTA);
        }

        // lcd_show_string(5, 170, 200, 16, 16, "KEY0:Send data", MAGENTA);
        // lcd_show_string(5, 190, lcddev.width - 30, lcddev.height - 190, 16, "Receive Data:", BLUE); /* ��ʾ��Ϣ */
    }
}

/**
 * @breif       freertos_demo
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{
    /* start_task���� */
    xTaskCreate((TaskFunction_t)start_task,
                (char const *)"start_task",
                (uint16_t)START_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIO,
                (TaskHandle_t *)&StartTask_Handler);

    vTaskStartScheduler(); /* ����������� */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;

    g_lwipdev.lwip_display_fn = lwip_test_ui;

    lwip_test_ui(1); /* ���غ�ǰ����UI */

    while (lwip_comm_init() != 0)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{1000});
    }

    while (g_lwipdev.dhcpstatus != 2 && g_lwipdev.dhcpstatus != 0xff) /* �ȴ���̬�Ͷ�̬�������  */
    {
        vTaskDelay(500);
    }

    taskENTER_CRITICAL(); /* �����ٽ��� */

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

    /* LED�������� */
    xTaskCreate((TaskFunction_t)led_task,
                (char const *)"led_task",
                (uint16_t)LED_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED_TASK_PRIO,
                (TaskHandle_t *)&LEDTask_Handler);

    /* ��ʾ���� */
    xTaskCreate((TaskFunction_t)display_task,
                (char const *)"display_task",
                (uint16_t)DISPLAY_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)DISPLAY_TASK_PRIO,
                (TaskHandle_t *)&DISPLAYTask_Handler);

    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
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
    // pvParameters = pvParameters;

    // uint8_t key;

    // while (1)
    // {
    //     key = key_scan(0);

    //     if (KEY0_PRES == key)
    //     {
    //         g_lwip_send_flag |= LWIP_SEND_DATA; /* ���LWIP������Ҫ���� */
    //     }

    //     vTaskDelay(10);
    // }
}

/**
 * @brief       ϵͳ������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void led_task(void *pvParameters)
{
    // pvParameters = pvParameters;

    // while (1)
    // {
    //     LED1_TOGGLE();
    //     vTaskDelay(100);
    // }
}

/**
 * @brief       ��ʾ����
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void display_task(void *pvParameters)
{
    // pvParameters = pvParameters;
    // uint8_t *buffer;

    // while (1)
    // {
    //     buffer = mymalloc(SRAMIN, 200);

    //     if (g_display_queue != NULL)
    //     {
    //         memset(buffer, 0, 200); /* ��������� */

    //         if (xQueueReceive(g_display_queue, buffer, portMAX_DELAY))
    //         {
    //             lcd_fill(30, 230, lcddev.width - 1, lcddev.height - 1, WHITE); /* ����һ������ */
    //             /* ��ʾ���յ������� */
    //             lcd_show_string(30, 230, lcddev.width - 30, lcddev.height - 230, 16, (char *)buffer, RED);
    //         }
    //     }

    //     myfree(SRAMIN, buffer); /*�ͷ��ڴ� */

    //     vTaskDelay(5);
    // }
}
