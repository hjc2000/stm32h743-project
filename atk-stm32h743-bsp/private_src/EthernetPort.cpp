#include "EthernetPort.h"
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

bsp::EthernetPort &bsp::EthernetPort::Instance()
{
	class Getter :
		public base::SingletonGetter<EthernetPort>
	{
	public:
		std::unique_ptr<EthernetPort> Create() override
		{
			return std::unique_ptr<EthernetPort>{new EthernetPort{}};
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

std::string bsp::EthernetPort::Name() const
{
	return "eth";
}

void bsp::EthernetPort::Open(base::Mac const &mac)
{
	_phy_driver.HardwareReset();

	// 打开以太网控制器
	_controller->Open(bsp::EthernetInterfaceType::RMII,
					  0,
					  mac);

	DI_TaskManager().Create(
		[this]()
		{
			while (true)
			{
				base::ReadOnlySpan span = Receive();
				try
				{
					_receiving_ethernet_frame_event.Invoke(span);
				}
				catch (...)
				{
				}
			}
		},
		1024);

	DI_TaskManager().Create(
		[this]()
		{
			bool last_loop_is_linked = false;
			while (true)
			{
				bool is_linked = _phy_driver.IsLinked();
				try
				{
					if (!last_loop_is_linked && is_linked)
					{
						_phy_driver.SoftwareReset();
						_phy_driver.EnableAutoNegotiation();

						// 启动以太网
						_controller->Start(_phy_driver.DuplexMode(), _phy_driver.Speed());

						_connection_event.Invoke();
					}
					else if (last_loop_is_linked && !is_linked)
					{
						_disconnection_event.Invoke();
					}
				}
				catch (...)
				{
				}

				last_loop_is_linked = is_linked;
				DI_Delayer().Delay(std::chrono::milliseconds{200});
			}
		},
		1024);
}

void bsp::EthernetPort::Send(base::IEnumerable<base::ReadOnlySpan> const &spans)
{
	_controller->Send(spans);
}

void bsp::EthernetPort::Send(base::ReadOnlySpan const &span)
{
	_controller->Send(span);
}

base::ReadOnlySpan bsp::EthernetPort::Receive()
{
	return _controller->Receive();
}

base::IEvent<base::ReadOnlySpan> &bsp::EthernetPort::ReceivingEhternetFrameEvent()
{
	return _receiving_ethernet_frame_event;
}

base::IEvent<> &bsp::EthernetPort::ConnectionEvent()
{
	return _connection_event;
}

base::IEvent<> &bsp::EthernetPort::DisconnectionEvent()
{
	return _disconnection_event;
}
