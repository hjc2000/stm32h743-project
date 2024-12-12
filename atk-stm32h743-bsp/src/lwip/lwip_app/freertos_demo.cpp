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

    lwip_demo(); /* lwip测试代码 */
}
