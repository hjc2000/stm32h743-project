#include <AtkLAN8720A_EhternetPort.h>
#include <bsp-interface/di/ethernet.h>

/// @brief 以太网端口。
/// @return
bsp::IEthernetPort &DI_EthernetPort()
{
	return bsp::AtkLAN8720A_EhternetPort::Instance();
}
