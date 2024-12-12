#include "ethernet.h"
#include "ethernet_chip.h"
#include "lwip_comm.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/interrupt.h>
#include <EthernetController.h>

/**
 * @breif       获得网络芯片的速度模式
 * @param       无
 * @retval      1:100M
                0:10M
 */
uint8_t ethernet_chip_get_speed(void)
{
    uint8_t speed = 0;
    if (PHY_TYPE == LAN8720)
    {
        DI_Console().WriteLine("LAN8720");

        /* 从LAN8720的31号寄存器中读取网络速度和双工模式 */
        speed = ~((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS));
    }
    else if (PHY_TYPE == SR8201F)
    {
        DI_Console().WriteLine("SR8201F");

        /* 从SR8201F的0号寄存器中读取网络速度和双工模式 */
        speed = ((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 13);
    }
    else if (PHY_TYPE == YT8512C)
    {
        DI_Console().WriteLine("YT8512C");

        /* 从YT8512C的17号寄存器中读取网络速度和双工模式 */
        speed = ((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 14);
    }
    else if (PHY_TYPE == RTL8201)
    {
        DI_Console().WriteLine("RTL8201");

        /* 从RTL8201的16号寄存器中读取网络速度和双工模式 */
        speed = ((DI_EthernetController().ReadPHYRegister(ETH_CHIP_PHYSCSR) & ETH_CHIP_SPEED_STATUS) >> 1);
    }

    return speed;
}
