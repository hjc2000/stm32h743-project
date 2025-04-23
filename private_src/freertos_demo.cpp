#include "base/task/delay.h"
#include "base/task/task.h"
#include "lwip/sockets.h"

void freertos_demo()
{
	socklen_t sock_fd = socket(AF_INET, SOCK_DGRAM, 0); /* 建立一个新的socket连接 */

	sockaddr_in local_info{}; /* 定义Socket地址信息结构体 */
	local_info.sin_len = sizeof(local_info);
	local_info.sin_family = AF_INET;                /* IPv4地址 */
	local_info.sin_port = htons(8080);              /* 设置端口号 */
	local_info.sin_addr.s_addr = htons(INADDR_ANY); /* 设置本地IP地址 */

	bind(sock_fd, (sockaddr *)&local_info, sizeof(sockaddr_in));

	base::task::run("",
					1,
					1024 * 8,
					[local_info, sock_fd]() mutable
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

							base::task::Delay(std::chrono::milliseconds{100});
						}
					});

	/* 接收数据缓冲区 */
	std::unique_ptr<uint8_t[]> receiving_buffer{new uint8_t[200]{}};
	while (true)
	{
		recv(sock_fd, receiving_buffer.get(), 200, 0);
	}
}
