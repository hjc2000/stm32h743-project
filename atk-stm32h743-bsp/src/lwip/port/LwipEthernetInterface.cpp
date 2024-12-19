#include "LwipEthernetInterface.h"
#include <base/container/Dictionary.h>
#include <base/container/List.h>
#include <base/di/SingletonGetter.h>
#include <base/stream/ReadOnlySpan.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <FreeRTOS.h>
#include <hal.h>
#include <lwip/dhcp.h>
#include <lwip/init.h>
#include <lwip/mem.h>
#include <lwip/memp.h>
#include <lwip/tcpip.h>
#include <lwip/timeouts.h>
#include <lwip_convert.h>
#include <netif/etharp.h>
#include <stdint.h>
#include <stdio.h>
#include <task.h>

#pragma region 线程函数

void bsp::LwipEthernetInterface::InputThreadFunc()
{
	while (true)
	{
		base::IEnumerable<base::ReadOnlySpan> const &spans = _ethernet_port->Receive();
		pbuf *head_pbuf = nullptr;
		pbuf *current_pbuf = nullptr;

		for (base::ReadOnlySpan const &span : spans)
		{
			pbuf_custom *custom_pbuf = new pbuf_custom{};
			custom_pbuf->custom_free_function = [](pbuf *p)
			{
				delete reinterpret_cast<pbuf_custom *>(p);
			};

			pbuf *p = pbuf_alloced_custom(PBUF_RAW,
										  span.Size(),
										  PBUF_REF,
										  custom_pbuf,
										  const_cast<uint8_t *>(span.Buffer()),
										  span.Size());

			p->next = nullptr;
			if (head_pbuf == nullptr)
			{
				head_pbuf = p;
				current_pbuf = p;
			}
			else
			{
				current_pbuf->next = p;
				current_pbuf = p;
			}
		}

		// 串成链表后一次性输入。
		if (_netif_wrapper->input(head_pbuf, _netif_wrapper) != err_enum_t::ERR_OK)
		{
			pbuf_free(head_pbuf);
		}
	}
}

void bsp::LwipEthernetInterface::LinkStateDetectingThreadFunc()
{
	while (true)
	{
		bool is_linked = _ethernet_port->IsLinked();
		if (is_linked == netif_is_up(_netif_wrapper))
		{
			DI_Delayer().Delay(std::chrono::milliseconds{100});
			continue;
		}

		if (is_linked)
		{
			// 开启以太网及虚拟网卡
			DI_Console().WriteLine("检测到网线插入");
			_ethernet_port->Restart();
			netif_set_up(_netif_wrapper);
			netif_set_link_up(_netif_wrapper);
			_netif_wrapper.TryDHCP();
		}
		else
		{
			_netif_wrapper.StopDHCP();

			/* LWIP_DHCP */
			DI_Console().WriteLine("检测到网线断开。");
			netif_set_down(_netif_wrapper);
			netif_set_link_down(_netif_wrapper);
		}
	}
}

#pragma endregion

bsp::LwipEthernetInterface &bsp::LwipEthernetInterface::Instance()
{
	class Getter :
		public base::SingletonGetter<LwipEthernetInterface>
	{
	public:
		std::unique_ptr<LwipEthernetInterface> Create() override
		{
			return std::unique_ptr<LwipEthernetInterface>{new LwipEthernetInterface{}};
		}

		void Lock() override
		{
			DI_DisableGlobalInterrupt();
		}

		void Unlock() override
		{
			DI_EnableGlobalInterrupt();
		}
	};

	Getter g;
	return g.Instance();
}

void bsp::LwipEthernetInterface::Open()
{
	_netif_wrapper.Open(_ethernet_port,
						_mac,
						_ip_address,
						_netmask,
						_gateway,
						ETH_MAX_PAYLOAD);

	_netif_wrapper.SetAsDefaultNetInterface();

	// 链接状态更新
	DI_TaskManager().Create(
		[this]()
		{
			LinkStateDetectingThreadFunc();
		},
		512);

	/* create the task that handles the ETH_MAC */
	DI_TaskManager().Create(
		[this]()
		{
			InputThreadFunc();
		},
		512);
}
