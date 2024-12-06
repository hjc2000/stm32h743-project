#include "sdram.h"
#include <bsp-interface/di/gpio.h>
#include <hal.h>

SDRAM_HandleTypeDef SDRAM_Handler{}; // SDRAM句柄

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    __HAL_RCC_FMC_CLK_ENABLE();

    char const *pin_names[] = {
        "PC0",
        "PC2",
        "PC3",
        "PD0",
        "PD1",
        "PD8",
        "PD9",
        "PD10",
        "PD14",
        "PD15",
        "PE0",
        "PE1",
        "PE7",
        "PE8",
        "PE9",
        "PE10",
        "PE11",
        "PE12",
        "PE13",
        "PE14",
        "PE15",
        "PF0",
        "PF1",
        "PF2",
        "PF3",
        "PF4",
        "PF5",
        "PF11",
        "PF12",
        "PF14",
        "PF15",
        "PG0",
        "PG1",
        "PG2",
        "PG4",
        "PG5",
        "PG8",
        "PG15",
    };

    for (char const *pin_name : pin_names)
    {
        bsp::IGpioPin *pin = DI_GpioPinCollection().Get(pin_name);
        pin->OpenAsAlternateFunctionMode("fmc",
                                         bsp::IGpioPinPullMode::PullUp,
                                         bsp::IGpioPinDriver::PushPull);
    }
}

// 发送SDRAM初始化序列
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    uint32_t temp = 0;

    // SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0);
    DI_Delayer().Delay(std::chrono::microseconds{500});
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_PALL, 1, 0);
    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);

    // 配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
    // bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
    // bit9为指定的写突发模式，bit10和bit11位保留位
    temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | // 设置突发长度:1(可以是1/2/4/8)
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |    // 设置突发类型:连续(可以是连续/交错)
           SDRAM_MODEREG_CAS_LATENCY_2 |            // 设置CAS值:3(可以是2/3)
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |  // 设置操作模式:0,标准模式
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;    // 设置突发写模式:1,单点访问

    SDRAM_Send_Cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, temp); // 设置SDRAM的模式寄存器

    // 刷新频率计数器(以SDCLK频率计数),计算方法:
    // COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
    // 我们使用的SDRAM刷新周期为64ms,SDCLK=200/2=100Mhz,行数为8192(2^13).
    // 所以,COUNT=64*1000*100/8192-20=761
    int const period = 64 * 1000 * 240 / 2 / 8192 - 20;
    HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler, period);
}

// SDRAM初始化
void SDRAM_Init(void)
{
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;

    SDRAM_Handler.Instance = FMC_SDRAM_DEVICE;
    SDRAM_Handler.Init.SDBank = FMC_SDRAM_BANK1;
    SDRAM_Handler.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
    SDRAM_Handler.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
    SDRAM_Handler.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
    SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    SDRAM_Handler.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
    SDRAM_Handler.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    SDRAM_Handler.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
    SDRAM_Handler.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
    SDRAM_Handler.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;
    SDRAM_Handler.MspInitCallback = HAL_SDRAM_MspInit;

    SDRAM_Timing.LoadToActiveDelay = 2;
    SDRAM_Timing.ExitSelfRefreshDelay = 8;
    SDRAM_Timing.SelfRefreshTime = 6;
    SDRAM_Timing.RowCycleDelay = 6;
    SDRAM_Timing.WriteRecoveryTime = 2;
    SDRAM_Timing.RPDelay = 2;
    SDRAM_Timing.RCDDelay = 2;
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
