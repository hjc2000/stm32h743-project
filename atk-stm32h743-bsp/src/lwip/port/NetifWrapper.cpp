#include "NetifWrapper.h"
#include <lwip/dhcp.h>

netif *lwip::NetifWrapper::WrappedObj() const
{
	return _wrapped_obj.get();
}

base::IPAddress lwip::NetifWrapper::IPAddress() const
{
	base::ReadOnlySpan span{
		reinterpret_cast<uint8_t const *>(&_wrapped_obj->ip_addr.addr),
		sizeof(_wrapped_obj->ip_addr.addr),
	};

	return base::IPAddress{std::endian::big, span};
}

void lwip::NetifWrapper::SetIPAddress(base::IPAddress const &ip_address)
{
	base::Span span{
		reinterpret_cast<uint8_t *>(&_wrapped_obj->ip_addr.addr),
		sizeof(_wrapped_obj->ip_addr.addr),
	};

	span.CopyFrom(ip_address.AsReadOnlySpan());

	/* base::IPAddress 类用小端序储存 IP 地址，而 lwip 的 net_interface.ip_addr.addr
	 * 是用大端序，所以要翻转。
	 */
	span.Reverse();
}

void lwip::NetifWrapper::StartDHCP()
{
	dhcp_start(_wrapped_obj.get());
}

void lwip::NetifWrapper::StopDHCP()
{
	dhcp_stop(_wrapped_obj.get());
}

void lwip::NetifWrapper::SetAsDefaultNetInterface()
{
	netif_set_default(_wrapped_obj.get());
}
