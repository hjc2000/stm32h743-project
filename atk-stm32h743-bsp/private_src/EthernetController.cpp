#include "EthernetController.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <EthernetController.h>
#include <hal.h>

#define ETH_CLK_GPIO_PORT GPIOA
#define ETH_CLK_GPIO_PIN GPIO_PIN_1

#define ETH_MDIO_GPIO_PORT GPIOA
#define ETH_MDIO_GPIO_PIN GPIO_PIN_2

#define ETH_CRS_GPIO_PORT GPIOA
#define ETH_CRS_GPIO_PIN GPIO_PIN_7

#define ETH_MDC_GPIO_PORT GPIOC
#define ETH_MDC_GPIO_PIN GPIO_PIN_1

#define ETH_RXD0_GPIO_PORT GPIOC
#define ETH_RXD0_GPIO_PIN GPIO_PIN_4

#define ETH_RXD1_GPIO_PORT GPIOC
#define ETH_RXD1_GPIO_PIN GPIO_PIN_5

#define ETH_TX_EN_GPIO_PORT GPIOB
#define ETH_TX_EN_GPIO_PIN GPIO_PIN_11

#define ETH_TXD0_GPIO_PORT GPIOG
#define ETH_TXD0_GPIO_PIN GPIO_PIN_13

#define ETH_TXD1_GPIO_PORT GPIOG
#define ETH_TXD1_GPIO_PIN GPIO_PIN_14

uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE] __attribute__((section(".ARM.__at_0x30040200"))); /* Ethernet Receive Buffers */

bsp::EthernetController::EthernetController()
{
	// MPU 设置
	{
		MPU_Region_InitTypeDef MPU_InitStruct{};
		HAL_MPU_Disable();
		MPU_InitStruct.Enable = MPU_REGION_ENABLE;
		MPU_InitStruct.BaseAddress = 0x30040000;
		MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
		MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
		MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
		MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
		MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
		MPU_InitStruct.Number = MPU_REGION_NUMBER5;
		MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
		MPU_InitStruct.SubRegionDisable = 0x00;
		MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
		HAL_MPU_ConfigRegion(&MPU_InitStruct);
		HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
	}

	// GPIO 初始化
	{
		GPIO_InitTypeDef gpio_init_struct;

		__HAL_RCC_GPIOA_CLK_ENABLE(); /* 开启ETH_CLK时钟 */
		__HAL_RCC_GPIOA_CLK_ENABLE(); /* 开启ETH_MDIO时钟 */
		__HAL_RCC_GPIOA_CLK_ENABLE(); /* 开启ETH_CRS时钟 */
		__HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启ETH_MDC时钟 */
		__HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启ETH_RXD0时钟 */
		__HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启ETH_RXD1时钟 */
		__HAL_RCC_GPIOB_CLK_ENABLE(); /* 开启ETH_TX_EN时钟 */
		__HAL_RCC_GPIOG_CLK_ENABLE(); /* 开启ETH_TXD0时钟 */
		__HAL_RCC_GPIOG_CLK_ENABLE(); /* 开启ETH_TXD1时钟 */

		/* Enable Ethernet clocks */
		__HAL_RCC_ETH1MAC_CLK_ENABLE();
		__HAL_RCC_ETH1TX_CLK_ENABLE();
		__HAL_RCC_ETH1RX_CLK_ENABLE();

		/* 网络引脚设置 RMII接口
		 * ETH_MDIO -------------------------> PA2
		 * ETH_MDC --------------------------> PC1
		 * ETH_RMII_REF_CLK------------------> PA1
		 * ETH_RMII_CRS_DV ------------------> PA7
		 * ETH_RMII_RXD0 --------------------> PC4
		 * ETH_RMII_RXD1 --------------------> PC5
		 * ETH_RMII_TX_EN -------------------> PB11
		 * ETH_RMII_TXD0 --------------------> PG13
		 * ETH_RMII_TXD1 --------------------> PG14
		 */

		/* PA1,2,7 */
		gpio_init_struct.Pin = ETH_CLK_GPIO_PIN;
		gpio_init_struct.Mode = GPIO_MODE_AF_PP;             /* 推挽复用 */
		gpio_init_struct.Pull = GPIO_NOPULL;                 /* 不带上下拉 */
		gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;       /* 高速 */
		gpio_init_struct.Alternate = GPIO_AF11_ETH;          /* 复用为ETH功能 */
		HAL_GPIO_Init(ETH_CLK_GPIO_PORT, &gpio_init_struct); /* ETH_CLK引脚模式设置 */

		gpio_init_struct.Pin = ETH_MDIO_GPIO_PIN;
		HAL_GPIO_Init(ETH_MDIO_GPIO_PORT, &gpio_init_struct); /* ETH_MDIO引脚模式设置 */

		gpio_init_struct.Pin = ETH_CRS_GPIO_PIN;
		HAL_GPIO_Init(ETH_CRS_GPIO_PORT, &gpio_init_struct); /* ETH_CRS引脚模式设置 */

		/* PC1 */
		gpio_init_struct.Pin = ETH_MDC_GPIO_PIN;
		HAL_GPIO_Init(ETH_MDC_GPIO_PORT, &gpio_init_struct); /* ETH_MDC初始化 */

		/* PC4 */
		gpio_init_struct.Pin = ETH_RXD0_GPIO_PIN;
		HAL_GPIO_Init(ETH_RXD0_GPIO_PORT, &gpio_init_struct); /* ETH_RXD0初始化 */

		/* PC5 */
		gpio_init_struct.Pin = ETH_RXD1_GPIO_PIN;
		HAL_GPIO_Init(ETH_RXD1_GPIO_PORT, &gpio_init_struct); /* ETH_RXD1初始化 */

		/* PB11,PG13,PG14 */
		gpio_init_struct.Pin = ETH_TX_EN_GPIO_PIN;
		HAL_GPIO_Init(ETH_TX_EN_GPIO_PORT, &gpio_init_struct); /* ETH_TX_EN初始化 */

		gpio_init_struct.Pin = ETH_TXD0_GPIO_PIN;
		HAL_GPIO_Init(ETH_TXD0_GPIO_PORT, &gpio_init_struct); /* ETH_TXD0初始化 */

		gpio_init_struct.Pin = ETH_TXD1_GPIO_PIN;
		HAL_GPIO_Init(ETH_TXD1_GPIO_PORT, &gpio_init_struct); /* ETH_TXD1初始化 */
	}
}

bsp::EthernetController &bsp::EthernetController::Instance()
{
	class Getter :
		public base::SingletonGetter<EthernetController>
	{
	public:
		std::unique_ptr<EthernetController> Create() override
		{
			return std::unique_ptr<EthernetController>{new EthernetController{}};
		}

		void Lock() override
		{
			DI_DisableGlobalInterrupt();
		}

		void Unlock() override
		{
			DI_EnableGlobalInterrupt();
		}
	};

	Getter g;
	return g.Instance();
}

std::string bsp::EthernetController::Name() const
{
	return "eth";
}

void bsp::EthernetController::Open(bsp::Ethernet_InterfaceType interface_type,
								   uint32_t phy_address,
								   base::Mac const &mac)
{
	_interface_type = interface_type;
	_phy_address = phy_address;
	_mac = mac;

	uint8_t big_endian_mac_buffer[6];
	base::Span big_endian_mac_buffer_span{big_endian_mac_buffer, sizeof(big_endian_mac_buffer)};
	big_endian_mac_buffer_span.CopyFrom(mac.AsReadOnlySpan());
	big_endian_mac_buffer_span.Reverse();

	_handle.Instance = ETH;
	_handle.Init.MACAddr = big_endian_mac_buffer;
	_handle.Init.MediaInterface = HAL_ETH_RMII_MODE;
	_handle.Init.RxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(0x30040000);
	_handle.Init.TxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(0x30040000 + 4 * sizeof(ETH_DMADescTypeDef));
	_handle.Init.RxBuffLen = ETH_MAX_PACKET_SIZE;

	HAL_StatusTypeDef result = HAL_ETH_Init(&_handle);
	if (result != HAL_OK)
	{
		throw std::runtime_error{"打开以太网口失败"};
	}

	// MDC时钟
	HAL_ETH_SetMDIOClockRange(&_handle);

	for (int idx = 0; idx < static_cast<int>(ETH_RX_DESC_CNT); idx++)
	{
		HAL_ETH_DescAssignMemory(&_handle,
								 idx,
								 Rx_Buff[idx],
								 NULL);
	}

	/* 这里的中断优先级必须设置在 freertos 能够屏蔽的优先级范围内，不然不知道什么原因，
	 * 会导致 freertos 的 queue.c 中报错。
	 */
	DI_EnableInterrupt(static_cast<uint32_t>(ETH_IRQn), 7);
	DI_IsrManager().AddIsr(static_cast<uint32_t>(ETH_IRQn),
						   [&]()
						   {
							   HAL_ETH_IRQHandler(&_handle);
						   });
}

uint32_t bsp::EthernetController::ReadPHYRegister(uint32_t register_index)
{
	uint32_t regval = 0;
	HAL_StatusTypeDef result = HAL_ETH_ReadPHYRegister(&_handle,
													   _phy_address,
													   register_index,
													   &regval);

	if (result != HAL_StatusTypeDef ::HAL_OK)
	{
		throw std::runtime_error{"读 PHY 寄存器失败"};
	}

	return regval;
}

void bsp::EthernetController::WritePHYRegister(uint32_t register_index, uint32_t value)
{
	HAL_StatusTypeDef result = HAL_ETH_WritePHYRegister(&_handle,
														_phy_address,
														register_index,
														value);

	if (result != HAL_StatusTypeDef ::HAL_OK)
	{
		throw std::runtime_error{"写 PHY 寄存器失败"};
	}
}

void bsp::EthernetController::Start(bsp::Ethernet_DuplexMode duplex_mode, base::Bps const &speed)
{
	ETH_MACConfigTypeDef def{};
	HAL_ETH_GetMACConfig(&_handle, &def);

	if (speed == static_cast<base::Bps>(base::Mbps{10}))
	{
		def.Speed = ETH_SPEED_10M;
	}
	else if (speed == static_cast<base::Bps>(base::Mbps{100}))
	{
		def.Speed = ETH_SPEED_100M;
	}
	else
	{
		throw std::invalid_argument{"不支持的速率"};
	}

	if (duplex_mode == bsp::Ethernet_DuplexMode::FullDuplex)
	{
		def.DuplexMode = ETH_FULLDUPLEX_MODE;
	}
	else
	{
		def.DuplexMode = ETH_HALFDUPLEX_MODE;
	}

	HAL_ETH_SetMACConfig(&_handle, &def);
	HAL_ETH_Start(&_handle);
}
