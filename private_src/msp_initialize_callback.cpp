#include "base/embedded/ethernet/ethernet_controller_handle.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/embedded/sdram/sdram_controller_handle.h"
#include "base/embedded/serial/serial_handle.h"
#include "base/embedded/timer/pwm_timer_handle.h"
#include "hal.h" // IWYU pragma: keep
#include <cstdint>
#include <vector>

void base::sdram::msp_initialize_callback(uint32_t id)
{
	std::vector<base::gpio::GpioPin> pins{
		base::gpio::GpioPin{base::gpio::PortEnum::PortC, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortC, 2},
		base::gpio::GpioPin{base::gpio::PortEnum::PortC, 3},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 8},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 9},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 10},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 14},
		base::gpio::GpioPin{base::gpio::PortEnum::PortD, 15},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 7},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 8},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 9},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 10},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 11},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 12},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 13},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 14},
		base::gpio::GpioPin{base::gpio::PortEnum::PortE, 15},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 2},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 3},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 4},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 5},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 11},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 12},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 13},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 14},
		base::gpio::GpioPin{base::gpio::PortEnum::PortF, 15},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 0},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 1},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 2},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 4},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 5},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 8},
		base::gpio::GpioPin{base::gpio::PortEnum::PortG, 15},
	};

	for (auto &pin : pins)
	{
		pin.InitializeAsAlternateFunctionMode(GPIO_AF12_FMC,
											  base::gpio::PullMode::PullUp,
											  base::gpio::DriveMode::PushPull);
	}
}

void base::serial::msp_initialize_callback(uint32_t id)
{
	base::gpio::GpioPin pa9{base::gpio::PortEnum::PortA, 9};
	base::gpio::GpioPin pa10{base::gpio::PortEnum::PortA, 10};

	// 发送引脚 PA9
	pa9.InitializeAsAlternateFunctionMode(GPIO_AF7_USART1,
										  base::gpio::PullMode::PullUp,
										  base::gpio::DriveMode::PushPull);

	// 接收引脚 PA10
	pa10.InitializeAsAlternateFunctionMode(GPIO_AF7_USART1,
										   base::gpio::PullMode::PullUp,
										   base::gpio::DriveMode::PushPull);
}

void base::ethernet::msp_initialize_callback(uint32_t id)
{
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

void base::pwm_timer::msp_initialize_callback(uint32_t id)
{
	base::gpio::GpioPin pin{base::gpio::PortEnum::PortB, 1};

	pin.InitializeAsAlternateFunctionMode(GPIO_AF2_TIM3,
										  base::gpio::PullMode::NoPull,
										  base::gpio::DriveMode::PushPull);
}
