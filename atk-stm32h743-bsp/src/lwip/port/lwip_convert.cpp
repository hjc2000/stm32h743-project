#include "lwip_convert.h"

template <>
ip_addr_t base::Convert(base::IPAddress const &o)
{
	ip_addr_t ip{};

	base::Span span{
		reinterpret_cast<uint8_t *>(&ip.addr),
		sizeof(ip.addr),
	};

	span.CopyFrom(o.AsReadOnlySpan());

	/* base::IPAddress 类用小端序储存 IP 地址，而 lwip 的 ip_addr.addr
	 * 是用大端序，所以要翻转。
	 */
	span.Reverse();
	return ip;
}

template <>
base::IPAddress base::Convert(ip_addr_t const &o)
{
	base::ReadOnlySpan span{
		reinterpret_cast<uint8_t const *>(&o.addr),
		sizeof(o.addr),
	};

	return base::IPAddress{std::endian::big, span};
}
