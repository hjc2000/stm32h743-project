#include "LwipEthernetInterface.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <ethernetif.h>
#include <FreeRTOS.h>
#include <hal.h>
#include <lwip/dhcp.h>
#include <lwip/init.h>
#include <lwip/mem.h>
#include <lwip/memp.h>
#include <lwip/tcpip.h>
#include <lwip/timeouts.h>
#include <netif/etharp.h>
#include <stdint.h>
#include <stdio.h>
#include <task.h>

bsp::LwipEthernetInterface::LwipEthernetInterface()
{
}

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
}
