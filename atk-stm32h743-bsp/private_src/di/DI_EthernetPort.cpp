#include <AtkApolloV1EthernetPort.h>
#include <AtkApolloV2EthernetPort.h>
#include <bsp-interface/di/ethernet.h>

/// @brief 以太网端口。
/// @return
bsp::IEthernetPort &DI_EthernetPort()
{
	return bsp::AtkApolloV1EthernetPort::Instance();
	// return bsp::AtkApolloV2EthernetPort::Instance();
}
