/**
 ****************************************************************************************************
 * @file        freertos_demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-08-01
 * @brief       lwIP SOCKET UDP 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 H743开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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
#include <lwip_demo.h>

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 5            /* 任务优先级 */
#define START_STK_SIZE 1024          /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;      /* 任务句柄 */
void start_task(void *pvParameters); /* 任务函数 */

/* LWIP_DEMO 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LWIP_DMEO_TASK_PRIO 11           /* 任务优先级 */
#define LWIP_DMEO_STK_SIZE 1024          /* 任务堆栈大小 */
TaskHandle_t LWIP_Task_Handler;          /* 任务句柄 */
void lwip_demo_task(void *pvParameters); /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO 10           /* 任务优先级 */
#define LED_STK_SIZE 1024          /* 任务堆栈大小 */
TaskHandle_t LEDTask_Handler;      /* 任务句柄 */
void led_task(void *pvParameters); /* 任务函数 */

/* KEY_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define KEY_TASK_PRIO 13           /* 任务优先级 */
#define KEY_STK_SIZE 1024          /* 任务堆栈大小 */
TaskHandle_t KEYTask_Handler;      /* 任务句柄 */
void key_task(void *pvParameters); /* 任务函数 */

/* DISPLAY_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define DISPLAY_TASK_PRIO 7            /* 任务优先级 */
#define DISPLAY_STK_SIZE 1024          /* 任务堆栈大小 */
TaskHandle_t DISPLAYTask_Handler;      /* 任务句柄 */
void display_task(void *pvParameters); /* 任务函数 */

/* 显示消息队列的数量 */
#define DISPLAYMSG_Q_NUM 20    /* 显示消息队列的数量 */
QueueHandle_t g_display_queue; /* 显示消息队列句柄 */

/******************************************************************************************************/

/**
 * @breif       freertos_demo
 * @param       无
 * @retval      无
 */
void freertos_demo()
{
    /* start_task任务 */
    xTaskCreate((TaskFunction_t)start_task,
                (char const *)"start_task",
                (uint16_t)START_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIO,
                (TaskHandle_t *)&StartTask_Handler);
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while (lwip_comm_init() != 0)
    {
        vTaskDelay(500);
    }

    while (g_lwipdev.dhcpstatus != 2 && g_lwipdev.dhcpstatus != 0xff) /* 等待静态和动态分配完成  */
    {
        vTaskDelay(500);
    }

    taskENTER_CRITICAL(); /* 进入临界区 */

    g_display_queue = xQueueCreate(DISPLAYMSG_Q_NUM, 200); /* 创建消息Message_Queue,队列项长度是200长度 */

    /* 创建lwIP任务 */
    xTaskCreate((TaskFunction_t)lwip_demo_task,
                (char const *)"lwip_demo_task",
                (uint16_t)LWIP_DMEO_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LWIP_DMEO_TASK_PRIO,
                (TaskHandle_t *)&LWIP_Task_Handler);

    /* key任务 */
    xTaskCreate((TaskFunction_t)key_task,
                (char const *)"key_task",
                (uint16_t)KEY_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)KEY_TASK_PRIO,
                (TaskHandle_t *)&KEYTask_Handler);

    /* LED测试任务 */
    xTaskCreate((TaskFunction_t)led_task,
                (char const *)"led_task",
                (uint16_t)LED_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED_TASK_PRIO,
                (TaskHandle_t *)&LEDTask_Handler);

    /* 显示任务 */
    xTaskCreate((TaskFunction_t)display_task,
                (char const *)"display_task",
                (uint16_t)DISPLAY_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)DISPLAY_TASK_PRIO,
                (TaskHandle_t *)&DISPLAYTask_Handler);

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       lwIP运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;

    lwip_demo(); /* lwip测试代码 */

    while (1)
    {
        vTaskDelay(5);
    }
}

/**
 * @brief       key_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void key_task(void *pvParameters)
{
    pvParameters = pvParameters;
    while (1)
    {
        g_lwip_send_flag |= LWIP_SEND_DATA; /* 标记LWIP有数据要发送 */
        vTaskDelay(10000);
    }
}

/**
 * @brief       系统再运行
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while (1)
    {
        vTaskDelay(100);
    }
}

/**
 * @brief       显示任务
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
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
