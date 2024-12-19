#include "NetifWrapper.h"
#include <bsp-interface/di/console.h>
#include <lwip/dhcp.h>
#include <lwip/etharp.h>
#include <lwip/tcpip.h>
#include <lwip_convert.h>

void lwip::NetifWrapper::InitializationCallbackFunc()
{
	_wrapped_obj->hostname = "lwip";
	_wrapped_obj->name[0] = 'p';
	_wrapped_obj->name[1] = 'n';

	// 设置 MAC 地址长度，为 6 个字节
	_wrapped_obj->hwaddr_len = ETHARP_HWADDR_LEN;

	SetMac(_init_callback_func_context._mac);
	_wrapped_obj->mtu = _init_callback_func_context._mtu;

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

lwip::NetifWrapper::NetifWrapper()
{
	_wrapped_obj->state = this;
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

	_init_callback_func_context._mac = mac;
	_init_callback_func_context._mtu = mtu;

	auto initialization_callback = [](netif *p) -> err_t
	{
		reinterpret_cast<NetifWrapper *>(p->state)->InitializationCallbackFunc();
		return err_enum_t::ERR_OK;
	};

	/* netif_add 函数的 state 参数是 lwip 用来让用户传递私有数据的，会被放到 netif 的 state 字段中，
	 * 这里传递了 this，这样就将 netif 和本类对象绑定了，只要拿到了 netif 指针，就能拿到本类对象的指针。
	 */
	netif *netif_add_result = netif_add(_wrapped_obj.get(),
										&ip_addr_t_ip_address,
										&ip_addr_t_netmask,
										&ip_addr_t_gataway,
										this,
										initialization_callback,
										tcpip_input);

	if (netif_add_result == nullptr)
	{
		DI_Console().WriteLine("添加网卡失败。");
		throw std::runtime_error{"添加网卡失败。"};
	}
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

bool lwip::NetifWrapper::DhcpSuppliedAddress()
{
	return dhcp_supplied_address(_wrapped_obj.get());
}

void lwip::NetifWrapper::SetAsDefaultNetInterface()
{
	netif_set_default(_wrapped_obj.get());
}
