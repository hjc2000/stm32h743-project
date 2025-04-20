#include "EthernetPort.h"
#include "base/task/delay.h"
#include "base/task/Mutex.h"
#include "base/unit/Mbps.h"
#include "bsp-interface/di/task.h"

void bsp::EthernetPort::Open(base::Mac const &mac)
{
	_phy_driver.HardwareReset();

	// 打开以太网控制器
	_controller.Initialize(base::ethernet::InterfaceType::RMII,
						   0,
						   mac);

	bsp::di::task::CreateTask(1024,
							  [this]()
							  {
								  while (true)
								  {
									  base::ReadOnlySpan span = _controller.Receive();
									  _receiving_ethernet_frame_event.Invoke(span);
								  }
							  });

	bsp::di::task::CreateTask(1024,
							  [this]()
							  {
								  bool last_loop_is_linked = false;
								  while (true)
								  {
									  bool is_linked = _phy_driver.IsLinked();
									  if (!last_loop_is_linked && is_linked)
									  {
										  _phy_driver.SoftwareReset();
										  _phy_driver.EnableAutoNegotiation();

										  // 启动以太网
										  _controller.Start(_phy_driver.DuplexMode(), base::Mbps{_phy_driver.Speed()});

										  _connected_event.Invoke();
									  }
									  else if (last_loop_is_linked && !is_linked)
									  {
										  _disconnected_event.Invoke();
									  }

									  last_loop_is_linked = is_linked;
									  base::task::Delay(std::chrono::milliseconds{200});
								  }
							  });
}

void bsp::EthernetPort::Send(std::vector<base::ReadOnlySpan> const &spans)
{
	base::task::MutexGuard l{_sending_lock};
	_controller.Send(spans);
}

void bsp::EthernetPort::Send(base::ReadOnlySpan const &span)
{
	base::task::MutexGuard l{_sending_lock};
	_controller.Send(span);
}

base::IEvent<base::ReadOnlySpan> &bsp::EthernetPort::ReceivingEhternetFrameEvent()
{
	return _receiving_ethernet_frame_event;
}

base::IEvent<> &bsp::EthernetPort::ConnectedEvent()
{
	return _connected_event;
}

base::IEvent<> &bsp::EthernetPort::DisconnectedEvent()
{
	return _disconnected_event;
}
