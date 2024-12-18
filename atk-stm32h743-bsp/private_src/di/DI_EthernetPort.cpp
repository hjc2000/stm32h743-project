#include <AtkEhternetPort.h>
#include <bsp-interface/di/ethernet.h>

/// @brief 以太网端口。
/// @return
bsp::IEthernetPort &DI_EthernetPort()
{
	return bsp::AtkEhternetPort::Instance();
}
