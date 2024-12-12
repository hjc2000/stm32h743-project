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
        bsp::IEthernetController_InterfaceType _interface_type;
        uint32_t _phy_address = 0;
        base::Mac _mac;

    public:
        static_function EthernetController &Instance();

        ETH_HandleTypeDef &Handle()
        {
            return _handle;
        }

        /// @brief 以太网控制器的名称。
        /// @return
        std::string Name() const override;

        /// @brief 获取本控制器当前使用的接口类型。
        /// @note 要打开本控制器后本属性才有效。
        /// @return
        bsp::IEthernetController_InterfaceType InterfaceType() const override
        {
            return _interface_type;
        }

        /// @brief 获取本控制器绑定的 PHY 地址。
        /// @return
        uint32_t PhyAddress() const override
        {
            return _phy_address;
        }

        /// @brief 获取本控制器绑定的 MAC 地址。
        /// @return
        base::Mac Mac() const override
        {
            return _mac;
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
    };

} // namespace bsp
