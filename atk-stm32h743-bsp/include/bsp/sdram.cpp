#include "sdram.h"
#include <base/unit/Hz.h>
#include <base/unit/MHz.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/sdram.h>
#include <bsp-interface/sdram/chip/W9825G6KH_6_TimingProvider.h>
#include <hal.h>

// SDRAM初始化
void SDRAM_Init(void)
{
	bsp::di::sdram::SDRAMController().OpenAsReadBurstMode(bsp::sdram::chip::W9825G6KH_6_TimingProvider{},
														  bsp::sdram::property::BankCount{4},
														  bsp::sdram::property::RowBitCount{13},
														  bsp::sdram::property::ColumnBitCount{9},
														  bsp::sdram::property::DataWidth{16},
														  bsp::sdram::property::ReadBurstLength{8});

	// SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
	bsp::di::Delayer().Delay(std::chrono::microseconds{500});
	bsp::di::sdram::SDRAMController().PrechargeAll();

	for (int i = 0; i < 8; i++)
	{
		bsp::di::sdram::SDRAMController().AutoRefresh();
	}

	/**
	 * 发送 “加载模式寄存器” 命令。
	 * 		@li 配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度
	 * 		@li bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
	 * 		@li bit9为指定的写突发模式，bit10和bit11位保留位
	 */
	uint32_t sdram_mod_register = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_8 | // 设置突发长度:1(可以是1/2/4/8)
								  SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |    // 设置突发类型:连续(可以是连续/交错)
								  SDRAM_MODEREG_CAS_LATENCY_2 |            // 设置CAS值:3(可以是2/3)
								  SDRAM_MODEREG_OPERATING_MODE_STANDARD |  // 设置操作模式:0,标准模式
								  SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;    // 设置突发写模式:1,单点访问

	bsp::di::sdram::SDRAMController().WriteModeRegister(sdram_mod_register);
}
