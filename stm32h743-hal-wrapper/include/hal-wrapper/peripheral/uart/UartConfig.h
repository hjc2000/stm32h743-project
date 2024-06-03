#pragma once
#include<bsp-interface/ISerial.h>
#include<hal.h>

namespace hal
{
	class UartConfig
	{
	public:
		UartConfig() = default;
		UartConfig(UartConfig const &o) = default;
		UartConfig(UART_InitTypeDef const &o);
		UartConfig &operator=(UartConfig const &o) = default;
		UartConfig &operator=(UART_InitTypeDef const &o);
		operator UART_InitTypeDef() const;

		/// <summary>
		///		波特率
		/// </summary>
		uint32_t _baud_rate = 115200;

		/// <summary>
		///		一个串行帧有多少位
		/// </summary>
		enum class WordLength
		{
			WordLength_7bit = UART_WORDLENGTH_7B,
			WordLength_8bit = UART_WORDLENGTH_8B,
			WordLength_9bit = UART_WORDLENGTH_9B,
		};

		/// <summary>
		///		一个串行帧有多少位
		/// </summary>
		WordLength _word_length = WordLength::WordLength_8bit;

		/// <summary>
		///		有多少位停止位
		/// </summary>
		enum class StopBitCount
		{
			StopBitCount_0_5_bit = UART_STOPBITS_0_5,
			StopBitCount_1_bit = UART_STOPBITS_1,
			StopBitCount_1_5_bit = UART_STOPBITS_1_5,
			StopBitCount_2_bit = UART_STOPBITS_2,
		};

		/// <summary>
		///		一个串行帧有多少个停止位
		/// </summary>
		StopBitCount _stop_bit_count = StopBitCount::StopBitCount_1_bit;

		/// <summary>
		///		UART 校验模式
		/// </summary>
		enum class Parity
		{
			None = UART_PARITY_NONE,
			Even = UART_PARITY_EVEN,
			Odd = UART_PARITY_ODD,
		};

		/// <summary>
		///		校验模式。
		///		- 校验位被放到串行帧中除了停止位以外的最高位。
		/// </summary>
		Parity _parity = Parity::None;

		/// <summary>
		///		串口收发模式。
		/// </summary>
		enum class Mode
		{
			/// <summary>
			///		只接收
			/// </summary>
			RX = UART_MODE_RX,

			/// <summary>
			///		只发送
			/// </summary>
			TX = UART_MODE_TX,

			/// <summary>
			///		接收和发送都启用
			/// </summary>
			RX_TX = UART_MODE_TX_RX,
		};

		/// <summary>
		///		收发模式
		/// </summary>
		Mode _mode = Mode::RX_TX;

		/// <summary>
		///		硬件流控
		/// </summary>
		enum class HardwareFlowControl
		{
			None = UART_HWCONTROL_NONE,
			RTS = UART_HWCONTROL_RTS,
			CTS = UART_HWCONTROL_CTS,
			RTS_CTS = UART_HWCONTROL_RTS_CTS,
		};

		/// <summary>
		///		硬件流控
		/// </summary>
		HardwareFlowControl _hardware_flow_control = HardwareFlowControl::None;

		/// <summary>
		///		过采样倍率
		/// </summary>
		enum class OverSampling
		{
			OverSampling_16 = UART_OVERSAMPLING_16,
			OverSampling_8 = UART_OVERSAMPLING_8,
		};

		/// <summary>
		///		过采样倍率
		/// </summary>
		OverSampling _over_sampling = OverSampling::OverSampling_16;

		enum class OneBitSampling
		{
			Disable = UART_ONE_BIT_SAMPLE_DISABLE,
			Enable = UART_ONE_BIT_SAMPLE_ENABLE,
		};

		OneBitSampling _one_bit_sampling = OneBitSampling::Disable;

		enum class ClockPrescaler
		{
			DIV1 = UART_PRESCALER_DIV1,
			DIV2 = UART_PRESCALER_DIV2,
			DIV4 = UART_PRESCALER_DIV4,
			DIV6 = UART_PRESCALER_DIV6,
			DIV8 = UART_PRESCALER_DIV8,
			DIV10 = UART_PRESCALER_DIV10,
			DIV12 = UART_PRESCALER_DIV12,
			DIV16 = UART_PRESCALER_DIV16,
			DIV32 = UART_PRESCALER_DIV32,
			DIV64 = UART_PRESCALER_DIV64,
			DIV128 = UART_PRESCALER_DIV128,
			DIV256 = UART_PRESCALER_DIV256,
		};

		ClockPrescaler _clock_prescaler = ClockPrescaler::DIV1;

		void Serialize(bsp::ISerial const &serial);
		void Deserialize(bsp::ISerial &serial) const;
	};
}
