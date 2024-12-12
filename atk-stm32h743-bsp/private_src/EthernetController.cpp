#include "EthernetController.h"
#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>

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

void bsp::EthernetController::Open(bsp::IEthernetController_InterfaceType interface_type, base::Mac const &mac)
{
}

uint32_t bsp::EthernetController::ReadPHYRegister(uint32_t phy_address, uint32_t register_index)
{
    return 0;
}

void bsp::EthernetController::WritePHYRegister(uint32_t phy_address, uint32_t register_index, uint32_t value)
{
}

base::Bps bsp::EthernetController::Bitrate()
{
    return base::Bps();
}
