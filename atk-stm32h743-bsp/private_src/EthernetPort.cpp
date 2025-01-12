#include "EthernetPort.h"
#include <base/container/Dictionary.h>
#include <base/string/define.h>
#include <base/string/ToHexString.h>
#include <base/unit/Mbps.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/system_time.h>
#include <bsp-interface/di/task.h>
#include <bsp-interface/TaskSingletonGetter.h>

bsp::EthernetPort &bsp::EthernetPort::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<EthernetPort>
	{
	public:
		std::unique_ptr<EthernetPort> Create() override
		{
			return std::unique_ptr<EthernetPort>{new EthernetPort{}};
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

	DI_CreateTask(1024,
				  [this]()
				  {
					  while (true)
					  {
						  base::ReadOnlySpan span = _controller->Receive();
						  try
						  {
							  _receiving_ethernet_frame_event.Invoke(span);
						  }
						  catch (std::exception const &e)
						  {
							  DI_Console().WriteError(CODE_POS_STR + e.what());
						  }
						  catch (...)
						  {
							  DI_Console().WriteError(CODE_POS_STR + "接收线程发生未知错误。");
						  }
					  }
				  });

	DI_CreateTask(1024,
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
						  catch (std::exception const &e)
						  {
							  DI_Console().WriteError(CODE_POS_STR + e.what());
						  }
						  catch (...)
						  {
							  DI_Console().WriteError(CODE_POS_STR + "链路连接维护线程发生未知错误。");
						  }

						  last_loop_is_linked = is_linked;
						  DI_Delayer().Delay(std::chrono::milliseconds{200});
					  }
				  });
}

void bsp::EthernetPort::Send(base::IEnumerable<base::ReadOnlySpan> const &spans)
{
	_controller->Send(spans);
}

void bsp::EthernetPort::Send(base::ReadOnlySpan const &span)
{
	_controller->Send(span);
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
