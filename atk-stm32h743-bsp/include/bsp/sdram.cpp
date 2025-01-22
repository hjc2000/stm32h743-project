#include "sdram.h"
#include <base/unit/Hz.h>
#include <base/unit/MHz.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/sdram.h>
#include <bsp-interface/sdram/chip/W9825G6KH_6_Timing.h>
#include <hal.h>

SDRAM_HandleTypeDef SDRAM_Handler{}; // SDRAM句柄

// 发送SDRAM初始化序列
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
	// SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM

	/**
	 * 上电并提供稳定的时钟，并且发送空操作命令。
	 * @note 空操作命令其实就是让 SDRAM 不做任何工作的命令，让它不要乱动，因为 CLK 还在初始化。
	 */
	SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0);
	bsp::di::Delayer().Delay(std::chrono::microseconds{500});

	// 发送 "对所有 BANK 进行预充电" 的命令。
	SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_PALL, 1, 0);

	// 发送自动刷新命令。刷新 8 次。
	SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);

	/**
	 * 发送 “加载模式寄存器” 命令。
	 * 		@li 配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度
	 * 		@li bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
	 * 		@li bit9为指定的写突发模式，bit10和bit11位保留位
	 */
	uint32_t sdram_mod_register = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | // 设置突发长度:1(可以是1/2/4/8)
								  SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |    // 设置突发类型:连续(可以是连续/交错)
								  SDRAM_MODEREG_CAS_LATENCY_3 |            // 设置CAS值:3(可以是2/3)
								  SDRAM_MODEREG_OPERATING_MODE_STANDARD |  // 设置操作模式:0,标准模式
								  SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;    // 设置突发写模式:1,单点访问

	// 设置SDRAM的模式寄存器
	SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, sdram_mod_register);

	bsp::sdram::chip::W9825G6KH_6_Timing timing{base::MHz{bsp::di::clock::ClockSignalCollection().Get("hclk")->Frequency() / 2}};
	int refresh_count = timing.T_AutoRefreshCommand_CLK_Count() - 50;
	if (refresh_count < 50)
	{
		throw std::runtime_error{"FMC 的频率过低导致几乎一直都要处于发送自动刷新命令的状态。"};
	}

	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler, refresh_count);
}

// SDRAM初始化
void SDRAM_Init(void)
{
	bsp::sdram::chip::W9825G6KH_6_Timing timing{base::MHz{bsp::di::clock::ClockSignalCollection().Get("hclk")->Frequency() / 2}};
	bsp::di::sdram::SDRAMController().Open(timing);

	SDRAM_Handler.Instance = FMC_SDRAM_DEVICE;
	SDRAM_Handler.Init.SDBank = FMC_SDRAM_BANK1;
	SDRAM_Handler.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
	SDRAM_Handler.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
	SDRAM_Handler.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
	SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	SDRAM_Handler.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
	SDRAM_Handler.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	SDRAM_Handler.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	SDRAM_Handler.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
	SDRAM_Handler.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;

	FMC_SDRAM_TimingTypeDef SDRAM_Timing{};

	/**
	 * HCLK 的频率是 SYSCLK / 2 = 480 / 2 = 240MHz
	 * FMC 给 SDRAM 的时钟通过 SDRAM_Handler.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	 * 又设置成 2 分频了。所以 SDRAM 的 CLK 频率是 120MHz.
	 *
	 * 1/ 120MHz 大概是 8 纳秒。
	 */

	/**
	 * 加载模式寄存器到激活时间的延迟为2个时钟周期。
	 * 设置完 SDRAM 的模式寄存器后，至少需要延迟 LoadToActiveDelay 才能发送激活命令。
	 * 激活命令就是通过 RAS# 进行行选通，激活 BANK.
	 */
	SDRAM_Timing.LoadToActiveDelay = timing.T_RSC_CLK_Count();

	// 退出自刷新延迟
	SDRAM_Timing.ExitSelfRefreshDelay = timing.T_XSR_CLK_Count();

	// 自刷新时间。
	SDRAM_Timing.SelfRefreshTime = timing.T_RAS_CLK_Count();

	/**
	 * 行循环延迟。
	 * 即 2 次行激活之间的延迟或 2 个自动刷新命令之间的延迟。
	 */
	SDRAM_Timing.RowCycleDelay = timing.T_RC_CLK_Count();

	/**
	 * 写恢复延迟为2个时钟周期。
	 */
	SDRAM_Timing.WriteRecoveryTime = timing.T_WR_CLK_Count();

	/**
	 * 预充电延迟为2个时钟周期。
	 * 预充电后要至少延迟 RPDelay 个时钟周期后才能发送其他命令。
	 */
	SDRAM_Timing.RPDelay = timing.T_RP_CLK_Count();

	/**
	 * 行到列延迟为2个时钟周期。
	 * HAL 库注释中的 “Activate Command” 是指 BANK 激活命令。BANK 靠 RAS# 引脚激活。
	 * 读写靠列选通信号 CAS# 激活。RCDDelay 定义的是行选通到列选通之间的延迟。
	 */
	SDRAM_Timing.RCDDelay = timing.T_RCD_CLK_Count();
	HAL_SDRAM_Init(&SDRAM_Handler, &SDRAM_Timing);

	SDRAM_Initialization_Sequence(&SDRAM_Handler); // 发送SDRAM初始化序列
}

// 向SDRAM发送命令
// bankx:0,向BANK5上面的SDRAM发送指令
//       1,向BANK6上面的SDRAM发送指令
// cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
// refresh:自刷新次数
// regval:模式寄存器的定义
// 返回值:0,正常;1,失败.
uint8_t SDRAM_Send_Cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval)
{
	uint32_t target_bank = 0;
	FMC_SDRAM_CommandTypeDef Command;

	if (bankx == 0)
	{
		/**
		 * 这里的 BAKN1 不是指 SDRAM 中的 4 个 BANK 中的 BANK1，而是指 STM32 内部的 FMC 接口
		 * 的 BANK1.
		 */
		target_bank = FMC_SDRAM_CMD_TARGET_BANK1;
	}
	else if (bankx == 1)
	{
		target_bank = FMC_SDRAM_CMD_TARGET_BANK2;
	}

	Command.CommandMode = cmd;                                             // 命令
	Command.CommandTarget = target_bank;                                   // 目标SDRAM存储区域
	Command.AutoRefreshNumber = refresh;                                   // 自刷新次数
	Command.ModeRegisterDefinition = regval;                               // 要写入模式寄存器的值
	if (HAL_SDRAM_SendCommand(&SDRAM_Handler, &Command, 0XFFFF) == HAL_OK) // 向SDRAM发送命令
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
