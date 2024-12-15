#pragma once
#include <base/define.h>
#include <bsp-interface/di/task.h>
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
		bsp::Ethernet_InterfaceType _interface_type;
		uint32_t _phy_address = 0;
		base::Mac _mac;
		ETH_TxPacketConfig _send_config{};
		ETH_BufferTypeDef _hal_eth_buffer{};
		int32_t _total_send_length = 0;
		std::shared_ptr<bsp::IBinarySemaphore> _send_completion_signal = DICreate_BinarySemaphore();

	public:
		static_function EthernetController &Instance();

		/// @brief hal 句柄。
		/// @return
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
		bsp::Ethernet_InterfaceType InterfaceType() const override;

		/// @brief 获取本控制器绑定的 PHY 地址。
		/// @note 要打开本控制器后本属性才有效。
		/// @return
		uint32_t PhyAddress() const override;

		/// @brief 获取本控制器绑定的 MAC 地址。
		/// @note 要打开本控制器后本属性才有效。
		/// @return
		base::Mac Mac() const override;

		/// @brief 打开以太网控制器。
		/// @param interface_type 连接着 PHY 的接口类型。（使用的是 MII 还是 RMII）
		/// @param phy_address PHY 的地址。
		/// @param mac MAC 地址。
		void Open(bsp::Ethernet_InterfaceType interface_type,
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

		/// @brief 启动以太网。
		/// @param duplex_mode
		/// @param speed
		void Start(bsp::Ethernet_DuplexMode duplex_mode,
				   base::Bps const &speed) override;

		/// @brief 发送。
		/// @param span
		void Write(base::ReadOnlySpan const &span) override;

		/// @brief 将 Write 写入流中的数据实际发送出去。不调用本方法的话，这些数据可能仅仅是
		/// 存在于缓冲区中。
		void Flush() override;
	};

} // namespace bsp
