#include "AtkEhternetPort.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <base/string/ToHexString.h>
#include <base/unit/Mbps.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/system_time.h>
#include <hal.h>

uint32_t bsp::AtkEhternetPort::ReadPHYRegister(uint32_t register_index)
{
	return _controller->ReadPHYRegister(register_index);
}

void bsp::AtkEhternetPort::WritePHYRegister(uint32_t register_index, uint32_t value)
{
	_controller->WritePHYRegister(register_index, value);
}

bsp::AtkEhternetPort &bsp::AtkEhternetPort::Instance()
{
	class Getter :
		public base::SingletonGetter<AtkEhternetPort>
	{
	public:
		std::unique_ptr<AtkEhternetPort> Create() override
		{
			return std::unique_ptr<AtkEhternetPort>{new AtkEhternetPort{}};
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

std::string bsp::AtkEhternetPort::Name() const
{
	return "eth";
}

void bsp::AtkEhternetPort::Open(base::Mac const &mac)
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
}

void bsp::AtkEhternetPort::Restart()
{
	SoftwareResetPHY();
	EnableAutoNegotiation();

	// 启动以太网
	_controller->Start(DuplexMode(), Speed());
}

void bsp::AtkEhternetPort::ResetPHY()
{
	/* 公司的开发板是旧版的，复位需要先输出高电平，延时后输出低电平。
	 * 家里的开发板是新版的，复位需要先输出低电平，延时后输出高电平。
	 */

	// 硬件复位
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 1);
	DI_Delayer().Delay(std::chrono::milliseconds{100});

	// 复位结束
	DI_ExpandedIoPortCollection().Get("ex_io")->WriteBit(7, 0);
	DI_Delayer().Delay(std::chrono::milliseconds{100});
}

void bsp::AtkEhternetPort::Send(base::IEnumerable<base::ReadOnlySpan> const &spans)
{
	_controller->Send(spans);
}

base::IEnumerable<base::ReadOnlySpan> const &bsp::AtkEhternetPort::Receive()
{
	return _controller->Receive();
}
