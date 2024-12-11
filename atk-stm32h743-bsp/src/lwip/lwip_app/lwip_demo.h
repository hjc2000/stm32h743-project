/**
 ****************************************************************************************************
 * @file        lwip_demo.h
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

#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include <stdint.h>

#define LWIP_SEND_DATA 0X80      /* 定义有数据发送 */
extern uint8_t g_lwip_send_flag; /* 数据发送标志位 */

#ifdef __cplusplus
extern "C"
{
#endif
    void lwip_demo(void);

#ifdef __cplusplus
}
#endif
#endif /* _CLIENT_H */
