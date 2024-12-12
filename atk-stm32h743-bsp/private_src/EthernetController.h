#pragma once
#include <base/define.h>
#include <bsp-interface/ethernet/IEthernetController.h>
#include <hal.h>

namespace bsp
{
    class EthernetController :
        public bsp::IEthernetController
    {
    private:
        EthernetController();

        ETH_HandleTypeDef _handle{};
        uint32_t _phy_address = 0;

        static_function void MspInitCallback(ETH_HandleTypeDef *handle);

    public:
        static_function EthernetController &Instance();

        ETH_HandleTypeDef &Handle()
        {
            return _handle;
        }

        /// @brief 打开以太网控制器。
        /// @param interface_type 连接着 PHY 的接口类型。（使用的是 MII 还是 RMII）
        /// @param phy_address PHY 的地址。
        /// @param mac MAC 地址。
        void Open(bsp::IEthernetController_InterfaceType interface_type,
                  uint32_t phy_address,
                  base::Mac const &mac) override;

        /// @brief 读 PHY 的寄存器
        /// @param register_index 寄存器索引。
        /// @return
        uint32_t ReadPHYRegister(uint32_t register_index) override;

        /// @brief 写 PHY 的寄存器。
        /// @param register_index 寄存器索引。
        /// @param value
        void WritePHYRegister(uint32_t register_index, uint32_t value) override;

        /// @brief 获取此接口的连接速率。
        /// @return
        base::Bps Bitrate() override;
    };

} // namespace bsp
