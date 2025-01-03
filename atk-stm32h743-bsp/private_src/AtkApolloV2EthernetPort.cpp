#include "AtkApolloV2EthernetPort.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <base/string/ToHexString.h>
#include <base/unit/Mbps.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/system_time.h>
#include <bsp-interface/di/task.h>

bsp::AtkApolloV2EthernetPort &bsp::AtkApolloV2EthernetPort::Instance()
{
	class Getter :
		public base::SingletonGetter<AtkApolloV2EthernetPort>
	{
	public:
		std::unique_ptr<AtkApolloV2EthernetPort> Create() override
		{
			return std::unique_ptr<AtkApolloV2EthernetPort>{new AtkApolloV2EthernetPort{}};
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

std::string bsp::AtkApolloV2EthernetPort::Name() const
{
	return "eth";
}

void bsp::AtkApolloV2EthernetPort::Open(base::Mac const &mac)
{
	ResetPHY();

	// 打开以太网控制器
	_controller->Open(bsp::Ethernet_InterfaceType::RMII,
					  0,
					  mac);

	SoftwareResetPHY();
	EnableAutoNegotiation();

	// 启动以太网
	_controller->Start(DuplexMode(), Speed());

	DI_TaskManager().Create(
		[this]()
		{
			while (true)
			{
				base::ReadOnlySpan span = Receive();
				_receiving_ethernet_frame_event.Invoke(span);
			}
		},
		1024);
}

uint32_t bsp::AtkApolloV2EthernetPort::ReadPHYRegister(uint32_t register_index)
{
	return _controller->ReadPHYRegister(register_index);
}

void bsp::AtkApolloV2EthernetPort::WritePHYRegister(uint32_t register_index, uint32_t value)
{
	_controller->WritePHYRegister(register_index, value);
}

void bsp::AtkApolloV2EthernetPort::Restart()
{
	SoftwareResetPHY();
	EnableAutoNegotiation();

	// 启动以太网
	_controller->Start(DuplexMode(), Speed());
}

void bsp::AtkApolloV2EthernetPort::ResetPHY()
{
	/* 公司的开发板是旧版的，复位需要先输出高电平，延时后输出低电平。
	 * 家里的开发板是新版的，复位需要先输出低电平，延时后输出高电平。
	 */

	// 硬件复位
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
	DI_Delayer().Delay(std::chrono::milliseconds{100});

	// 复位结束
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
	DI_Delayer().Delay(std::chrono::milliseconds{100});
}

void bsp::AtkApolloV2EthernetPort::Send(base::IEnumerable<base::ReadOnlySpan> const &spans)
{
	_controller->Send(spans);
}

void bsp::AtkApolloV2EthernetPort::Send(base::ReadOnlySpan const &span)
{
	_controller->Send(span);
}

base::ReadOnlySpan bsp::AtkApolloV2EthernetPort::Receive()
{
	return _controller->Receive();
}

base::IEvent<base::ReadOnlySpan> &bsp::AtkApolloV2EthernetPort::ReceivintEhternetFrameEvent()
{
	return _receiving_ethernet_frame_event;
}
