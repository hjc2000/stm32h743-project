#include "ethernet_chip.h"
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/ethernet.h>

int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);
