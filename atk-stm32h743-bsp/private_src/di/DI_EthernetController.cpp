#include <bsp-interface/di/ethernet.h>
#include <EhternetPort.h>
#include <EthernetController.h>

/// @brief 以太网控制器。
/// @return
bsp::IEthernetController &DI_EthernetController()
{
    return bsp::EthernetController::Instance();
}

/// @brief 以太网端口。
/// @return
bsp::IEthernetPort &DI_EthernetPort()
{
    return bsp::EhternetPort::Instance();
}
