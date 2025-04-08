#include "base/define.h"
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/ethernet/MutexEthernetPort.h>
#include <EthernetPort.h>

PREINIT(bsp::di::ethernet::EthernetPort)

bsp::IEthernetPort &bsp::di::ethernet::EthernetPort()
{
	static bsp::MutexEthernetPort o{&bsp::EthernetPort::Instance()};
	return o;
}
