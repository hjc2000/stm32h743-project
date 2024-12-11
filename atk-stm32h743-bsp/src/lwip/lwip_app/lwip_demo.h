/**
 ****************************************************************************************************
 * @file        lwip_demo.h
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

#ifndef _LWIP_DEMO_H
#define _LWIP_DEMO_H
#include <stdint.h>

#define LWIP_SEND_DATA 0X80      /* ���������ݷ��� */
extern uint8_t g_lwip_send_flag; /* ���ݷ��ͱ�־λ */

#ifdef __cplusplus
extern "C"
{
#endif
    void lwip_demo(void);

#ifdef __cplusplus
}
#endif
#endif /* _CLIENT_H */
