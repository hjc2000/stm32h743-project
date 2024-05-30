#pragma once
#include<hal.h>

namespace hal
{
	struct PA9Alternate
	{
		static constinit uint32_t const usart1 = GPIO_AF7_USART1;
	};

	struct PA10Alternate
	{
		static constinit uint32_t const usart1 = GPIO_AF7_USART1;
	};
}