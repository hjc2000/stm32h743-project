#include "netif_extension.h"

void lwip::SetIPAddress(netif &net_interface, base::IPAddress const &ip_address)
{
	base::Span span{
		reinterpret_cast<uint8_t *>(&net_interface.ip_addr.addr),
		sizeof(net_interface.ip_addr.addr),
	};

	span.CopyFrom(ip_address.AsReadOnlySpan());

	/* base::IPAddress 类用小端序储存 IP 地址，而 lwip 的 net_interface.ip_addr.addr
	 * 是用大端序，所以要翻转。
	 */
	span.Reverse();
}
