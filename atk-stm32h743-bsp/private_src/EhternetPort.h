#pragma once
#include <base/define.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/ethernet/IEthernetPort.h>

namespace bsp
{
    enum class PhyRegister : uint32_t
    {
        BCR = 0,
        BSR = 1,
        REGISTER2 = 2,
        REGISTER3 = 3,
    };

    /// @brief 以太网端口。
    class EhternetPort :
        public bsp::IEthernetPort
    {
    private:
        bsp::IEthernetController *_controller = &DI_EthernetController();

        void ChipInitialize();

    public:
        static_function EhternetPort &Instance();

        /// @brief 以太网控制器的名称。
        /// @return
        std::string Name() const override;

        /// @brief 打开以太网端口。
        /// @param mac MAC 地址。
        void Open(base::Mac const &mac) override;

        /// @brief 读 PHY 的寄存器
        /// @param register_index 寄存器索引。
        /// @return
        uint32_t ReadPHYRegister(uint32_t register_index) override;

        /// @brief 写 PHY 的寄存器。
        /// @param register_index 寄存器索引。
        /// @param value
        void WritePHYRegister(uint32_t register_index, uint32_t value) override;

        /// @brief 复位 PHY 芯片。
        void ResetPHY() override;

        /// @brief 自动协商。
        void EnableAutoNegotiation() override;
    };
} // namespace bsp
