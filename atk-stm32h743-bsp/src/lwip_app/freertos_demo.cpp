#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <FreeRTOS.h>
#include <hal.h>
#include <lwip/api.h>
#include <lwip/opt.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwipopts.h>
#include <NetifWrapper.h>
#include <queue.h>
#include <semphr.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <task.h>

/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[200];

struct sockaddr_in local_info; /* 定义Socket地址信息结构体 */
socklen_t sock_fd;             /* 定义一个Socket接口 */

void freertos_demo()
{
	lwip::NetifWrapper _netif_wrapper;

	while (true)
	{
		try
		{
			_netif_wrapper.Open(&DI_EthernetPort(), ETH_MAX_PAYLOAD);
			break;
		}
		catch (std::exception const &e)
		{
			DI_Console().WriteLine(e.what());
			DI_Delayer().Delay(std::chrono::milliseconds{500});
		}
	}

	DI_TaskManager().Create(
		[]()
		{
			local_info.sin_addr.s_addr = inet_addr("192.168.1.203"); /* 需要发送的远程IP地址 */
			std::string sending_message{"UDP testing message"};

			while (true)
			{
				sendto(sock_fd,
					   sending_message.c_str(), // 发送的数据
					   sending_message.size(),  // 发送的数据大小
					   0,
					   (sockaddr *)&local_info, // 接收端地址信息
					   sizeof(local_info));     // 接收端地址信息大小

				DI_Delayer().Delay(std::chrono::milliseconds{100});
			}
		},
		512);

	memset(&local_info, 0, sizeof(struct sockaddr_in)); /* 将服务器地址清空 */
	local_info.sin_len = sizeof(local_info);
	local_info.sin_family = AF_INET;                /* IPv4地址 */
	local_info.sin_port = htons(8080);              /* 设置端口号 */
	local_info.sin_addr.s_addr = htons(INADDR_ANY); /* 设置本地IP地址 */

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0); /* 建立一个新的socket连接 */

	/* 建立绑定 */
	bind(sock_fd, (struct sockaddr *)&local_info, sizeof(struct sockaddr_in));
	while (true)
	{
		memset(g_lwip_demo_recvbuf, 0, sizeof(g_lwip_demo_recvbuf));
		recv(sock_fd, (void *)g_lwip_demo_recvbuf, sizeof(g_lwip_demo_recvbuf), 0);
	}
}
