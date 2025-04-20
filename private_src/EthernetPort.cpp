#include "EthernetPort.h"
#include "base/string/define.h"
#include "base/task/delay.h"
#include "base/unit/Mbps.h"
#include "bsp-interface/di/console.h"
#include "bsp-interface/di/task.h"

std::string bsp::EthernetPort::Name() const
{
	return "eth";
}

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
									  try
									  {
										  _receiving_ethernet_frame_event.Invoke(span);
									  }
									  catch (std::exception const &e)
									  {
										  bsp::di::Console().WriteError(CODE_POS_STR + e.what());
									  }
									  catch (...)
									  {
										  bsp::di::Console().WriteError(CODE_POS_STR + "接收线程发生未知错误。");
									  }
								  }
							  });

	bsp::di::task::CreateTask(1024,
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
											  _controller.Start(_phy_driver.DuplexMode(), base::Mbps{_phy_driver.Speed()});

											  _connection_event.Invoke();
										  }
										  else if (last_loop_is_linked && !is_linked)
										  {
											  _disconnection_event.Invoke();
										  }
									  }
									  catch (std::exception const &e)
									  {
										  bsp::di::Console().WriteError(CODE_POS_STR + e.what());
									  }
									  catch (...)
									  {
										  bsp::di::Console().WriteError(CODE_POS_STR + "链路连接维护线程发生未知错误。");
									  }

									  last_loop_is_linked = is_linked;
									  base::task::Delay(std::chrono::milliseconds{200});
								  }
							  });
}

void bsp::EthernetPort::Send(std::vector<base::ReadOnlySpan> const &spans)
{
	_controller.Send(spans);
}

void bsp::EthernetPort::Send(base::ReadOnlySpan const &span)
{
	_controller.Send(span);
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
