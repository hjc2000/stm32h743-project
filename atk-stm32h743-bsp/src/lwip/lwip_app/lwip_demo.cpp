#include "lwip_demo.h"
#include "FreeRTOS.h"
#include "lwip/api.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include <bsp-interface/di/delayer.h>
#include <lwip/sockets.h>
#include <stdint.h>
#include <stdio.h>

/* 需要自己设置远程IP地址 */
#define IP_ADDR "192.168.1.203"

#define LWIP_DEMO_RX_BUFSIZE 200                     /* 最大接收数据长度 */
#define LWIP_DEMO_PORT 8080                          /* 连接的本地端口号 */
#define LWIP_SEND_THREAD_PRIO (tskIDLE_PRIORITY + 3) /* 发送数据线程优先级 */

/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE];

/* 发送数据内容 */
char g_lwip_demo_sendbuf[] = "ALIENTEK UDP TEST\r\n";

/* 数据发送标志位 */
uint8_t g_lwip_send_flag;
struct sockaddr_in local_info; /* 定义Socket地址信息结构体 */
socklen_t sock_fd;             /* 定义一个Socket接口 */

static void lwip_send_thread(void *arg);

extern QueueHandle_t g_display_queue; /* 显示消息队列句柄 */

/**
 * @brief       发送数据线程
 * @param       无
 * @retval      无
 */
void lwip_data_send(void)
{
    sys_thread_new("lwip_send_thread",
                   lwip_send_thread,
                   NULL,
                   512,
                   LWIP_SEND_THREAD_PRIO);
}

/**
 * @brief       lwip_demo实验入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    BaseType_t lwip_err;
    lwip_data_send();                                   /* 创建发送数据线程 */
    memset(&local_info, 0, sizeof(struct sockaddr_in)); /* 将服务器地址清空 */
    local_info.sin_len = sizeof(local_info);
    local_info.sin_family = AF_INET;                /* IPv4地址 */
    local_info.sin_port = htons(LWIP_DEMO_PORT);    /* 设置端口号 */
    local_info.sin_addr.s_addr = htons(INADDR_ANY); /* 设置本地IP地址 */

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0); /* 建立一个新的socket连接 */

    /* 建立绑定 */
    bind(sock_fd, (struct sockaddr *)&local_info, sizeof(struct sockaddr_in));
    while (1)
    {
        memset(g_lwip_demo_recvbuf, 0, sizeof(g_lwip_demo_recvbuf));
        recv(sock_fd, (void *)g_lwip_demo_recvbuf, sizeof(g_lwip_demo_recvbuf), 0);

        lwip_err = xQueueSend(g_display_queue, &g_lwip_demo_recvbuf, 0);

        if (lwip_err == errQUEUE_FULL)
        {
            printf("队列Key_Queue已满，数据发送失败!\r\n");
        }
    }
}

/**
 * @brief       发送数据线程函数
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_send_thread(void *pvParameters)
{
    pvParameters = pvParameters;
    local_info.sin_addr.s_addr = inet_addr(IP_ADDR); /* 需要发送的远程IP地址 */
    while (true)
    {
        if ((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) /* 有数据要发送 */
        {
            sendto(sock_fd,                        /* scoket */
                   (char *)g_lwip_demo_sendbuf,    /* 发送的数据 */
                   sizeof(g_lwip_demo_sendbuf), 0, /* 发送的数据大小 */
                   (struct sockaddr *)&local_info, /* 接收端地址信息 */
                   sizeof(local_info));            /* 接收端地址信息大小 */

            g_lwip_send_flag &= ~LWIP_SEND_DATA;
        }

        DI_Delayer().Delay(std::chrono::milliseconds{100});
    }
}
