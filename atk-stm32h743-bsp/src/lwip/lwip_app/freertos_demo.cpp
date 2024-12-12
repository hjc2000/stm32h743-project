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
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/interrupt.h>
#include <lwip_demo.h>

/******************************************************************************************************/
/*FreeRTOS配置*/

/* LWIP_DEMO 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LWIP_DMEO_TASK_PRIO 11           /* 任务优先级 */
#define LWIP_DMEO_STK_SIZE 1024          /* 任务堆栈大小 */
TaskHandle_t LWIP_Task_Handler;          /* 任务句柄 */
void lwip_demo_task(void *pvParameters); /* 任务函数 */

/******************************************************************************************************/

/**
 * @breif       freertos_demo
 * @param       无
 * @retval      无
 */
void freertos_demo()
{
    while (lwip_comm_init() != 0)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{500});
    }

    DI_Console().WriteLine("lwip_comm_init successfully");

    while (g_lwipdev.dhcpstatus != 2 && g_lwipdev.dhcpstatus != 0xff) /* 等待静态和动态分配完成  */
    {
        DI_Delayer().Delay(std::chrono::milliseconds{500});
    }

    DI_DoGlobalCriticalWork([&]()
                            {
                                /* 创建lwIP任务 */
                                xTaskCreate((TaskFunction_t)lwip_demo_task,
                                            (char const *)"lwip_demo_task",
                                            (uint16_t)LWIP_DMEO_STK_SIZE,
                                            (void *)NULL,
                                            (UBaseType_t)LWIP_DMEO_TASK_PRIO,
                                            (TaskHandle_t *)&LWIP_Task_Handler);
                            });
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
