#pragma once
#include <base/net/IPAddress.h>
#include <lwip/netif.h>

namespace lwip
{
	void SetIPAddress(netif &net_interface, base::IPAddress const &ip_address);
}
