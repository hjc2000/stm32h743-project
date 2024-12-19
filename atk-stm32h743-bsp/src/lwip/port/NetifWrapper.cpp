#include "NetifWrapper.h"
#include <bsp-interface/di/console.h>
#include <lwip/dhcp.h>
#include <lwip/etharp.h>
#include <lwip/tcpip.h>
#include <lwip_convert.h>

lwip::NetifWrapper::NetifWrapper()
{
	_wrapped_obj->hostname = "lwip";
	_wrapped_obj->name[0] = 'p';
	_wrapped_obj->name[1] = 'n';

	// 设置 MAC 地址长度，为 6 个字节
	_wrapped_obj->hwaddr_len = ETHARP_HWADDR_LEN;
}

void lwip::NetifWrapper::Open(base::Mac const &mac,
							  base::IPAddress const &ip_address,
							  base::IPAddress const &netmask,
							  base::IPAddress const &gateway,
							  int32_t mtu)
{
	ip_addr_t ip_addr_t_ip_address = base::Convert<ip_addr_t, base::IPAddress>(ip_address);
	ip_addr_t ip_addr_t_netmask = base::Convert<ip_addr_t, base::IPAddress>(netmask);
	ip_addr_t ip_addr_t_gataway = base::Convert<ip_addr_t, base::IPAddress>(gateway);

	auto initialization_callback = [](netif *p) -> err_t
	{
		return err_enum_t::ERR_OK;
	};

	/* 将 _wrapped_obj 添加到 lwip 的链表。这个函数内部没有根据 _wrapped_obj 填写的做什么
	 * 初始化，所以完全可以在调用这个函数之后更改 _wrapped_obj 的字段，进行一些自定义的初始化。
	 * 所以没有必要依靠这个函数安排的回调函数进行初始化。
	 */
	netif *netif_add_result = netif_add(_wrapped_obj.get(),
										&ip_addr_t_ip_address,
										&ip_addr_t_netmask,
										&ip_addr_t_gataway,
										nullptr,
										initialization_callback,
										tcpip_input);

	if (netif_add_result == nullptr)
	{
		DI_Console().WriteLine("添加网卡失败。");
		throw std::runtime_error{"添加网卡失败。"};
	}

	SetMac(mac);
	_wrapped_obj->mtu = mtu;

	/* 网卡状态信息标志位，是很重要的控制字段，它包括网卡功能使能、广播
	 * 使能、 ARP 使能等等重要控制位
	 */
	_wrapped_obj->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...)
	 */
	_wrapped_obj->output = etharp_output;
}

netif *lwip::NetifWrapper::WrappedObj() const
{
	return _wrapped_obj.get();
}

base::Mac lwip::NetifWrapper::Mac() const
{
	return base::Mac{
		std::endian::big,
		base::ReadOnlySpan{
			_wrapped_obj->hwaddr,
			sizeof(_wrapped_obj->hwaddr),
		},
	};
}

void lwip::NetifWrapper::SetMac(base::Mac const &o)
{
	base::Span netif_mac_buff_span{_wrapped_obj->hwaddr, 6};
	netif_mac_buff_span.CopyFrom(o.AsReadOnlySpan());
	netif_mac_buff_span.Reverse();
}

base::IPAddress lwip::NetifWrapper::IPAddress() const
{
	return base::Convert<base::IPAddress, ip_addr_t>(_wrapped_obj->ip_addr);
}

void lwip::NetifWrapper::SetIPAddress(base::IPAddress const &value)
{
	_wrapped_obj->ip_addr = base::Convert<ip_addr_t, base::IPAddress>(value);

	netif_set_addr(_wrapped_obj.get(),
				   &_wrapped_obj->ip_addr,
				   &_wrapped_obj->netmask,
				   &_wrapped_obj->gw);
}

base::IPAddress lwip::NetifWrapper::Netmask() const
{
	return base::Convert<base::IPAddress, ip_addr_t>(_wrapped_obj->netmask);
}

void lwip::NetifWrapper::SetNetmask(base::IPAddress const &value)
{
	_wrapped_obj->netmask = base::Convert<ip_addr_t, base::IPAddress>(value);

	netif_set_addr(_wrapped_obj.get(),
				   &_wrapped_obj->ip_addr,
				   &_wrapped_obj->netmask,
				   &_wrapped_obj->gw);
}

base::IPAddress lwip::NetifWrapper::Gateway() const
{
	return base::Convert<base::IPAddress, ip_addr_t>(_wrapped_obj->gw);
}

void lwip::NetifWrapper::SetGateway(base::IPAddress const &value)
{
	_wrapped_obj->gw = base::Convert<ip_addr_t, base::IPAddress>(value);

	netif_set_addr(_wrapped_obj.get(),
				   &_wrapped_obj->ip_addr,
				   &_wrapped_obj->netmask,
				   &_wrapped_obj->gw);
}

void lwip::NetifWrapper::SetAllAddress(base::IPAddress const &ip_address,
									   base::IPAddress const &netmask,
									   base::IPAddress const &gateway)
{
	_wrapped_obj->ip_addr = base::Convert<ip_addr_t, base::IPAddress>(ip_address);
	_wrapped_obj->netmask = base::Convert<ip_addr_t, base::IPAddress>(netmask);
	_wrapped_obj->gw = base::Convert<ip_addr_t, base::IPAddress>(gateway);

	netif_set_addr(_wrapped_obj.get(),
				   &_wrapped_obj->ip_addr,
				   &_wrapped_obj->netmask,
				   &_wrapped_obj->gw);
}

void lwip::NetifWrapper::ClearAllAddress()
{
	_wrapped_obj->ip_addr = ip_addr_t{};
	_wrapped_obj->netmask = ip_addr_t{};
	_wrapped_obj->gw = ip_addr_t{};

	netif_set_addr(_wrapped_obj.get(),
				   &_wrapped_obj->ip_addr,
				   &_wrapped_obj->netmask,
				   &_wrapped_obj->gw);
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
