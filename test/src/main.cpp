#include <atomic>
#include <base/RentedPtrFactory.h>
#include <base/string/ToHexString.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/core.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/eerom.h>
#include <bsp-interface/di/expanded_io.h>
#include <bsp-interface/di/gpio.h>
#include <bsp-interface/di/iic.h>
#include <bsp-interface/di/led.h>
#include <bsp-interface/di/task.h>
#include <bsp-interface/flash/RmaFlash.h>
#include <bsp-interface/test/TestFlash.h>
#include <bsp-interface/test/TestIndependentWatchDog.h>
#include <bsp-interface/test/TestKeyScanner.h>
#include <bsp-interface/test/TestSerial.h>
#include <bsp/bsp.h>
#include <bsp/sdram.h>
#include <ff.h>
#include <littlefs/LfsFlashPort.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <TestExtiKey.h>

void TestLittleFs()
{
    Lfs::LfsFlashPort port{DI_FlashCollection().Get("internal-flash")};

    // 初始化 LittleFS 文件系统
    lfs_t lfs;
    int res = lfs_format(&lfs, &port.Port()); // 格式化文件系统
    if (res)
    {
        DI_Console().WriteLine("format failed: " + std::to_string(res));
    }

    res = lfs_mount(&lfs, &port.Port()); // 挂载文件系统
    if (res)
    {
        DI_Console().WriteLine("mount failed: " + std::to_string(res));
    }

    // 创建文件并写入数据
    char const *filename = "example.txt";
    char const *str = "Hello, LittleFS!";
    lfs_file_t file;

    res = lfs_file_open(&lfs, &file, filename, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
    if (res)
    {
        DI_Console().WriteLine("open failed: " + std::to_string(res));
    }

    res = lfs_file_write(&lfs, &file, str, strlen(str));
    if (res < 0)
    {
        DI_Console().WriteLine("write failed: " + std::to_string(res));
    }

    lfs_file_close(&lfs, &file);

    // 读取文件内容
    res = lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY);
    if (res)
    {
        DI_Console().WriteLine("open failed: " + std::to_string(res));
    }

    char buffer[128] = {0};
    res = lfs_file_read(&lfs, &file, buffer, sizeof(buffer) - 1);
    if (res < 0)
    {
        DI_Console().WriteLine("read failed: " + std::to_string(res));
    }

    DI_Console().WriteLine("have read: " + std::to_string(res));
    DI_Console().WriteLine(buffer);

    lfs_file_close(&lfs, &file);

    // 卸载文件系统
    lfs_unmount(&lfs);
}

inline void TestFatFs()
{
    FATFS fatfs{};
    BYTE work[FF_MAX_SS]; // 工作缓冲区
    FRESULT res{};

    // 初始化FatFs模块
    res = f_mount(&fatfs, "", 0); // 卸载任何已挂载的卷
    if (res != FR_OK)
    {
        DI_Console().WriteLine("f_mount error: " + std::to_string(res));
    }

    // 创建格式化参数结构体
    MKFS_PARM mkfs_parm{};
    mkfs_parm.fmt = FM_FAT; // 设置文件系统类型为FAT，并创建一个简单的FAT12/FAT16分区
    mkfs_parm.n_fat = 1;    // 设置FAT表的数量为1
    mkfs_parm.align = 1;    // 数据区域对齐为1扇区
    mkfs_parm.n_root = 0;   // 根目录条目数为0（自动选择）
    mkfs_parm.au_size = 0;  // 分配单元大小为自动选择

    // 格式化磁盘
    res = f_mkfs("", &mkfs_parm, work, sizeof(work));
    if (res != FR_OK)
    {
        DI_Console().WriteLine("f_mkfs error: " + std::to_string(res));
    }

    // 重新挂载文件系统
    res = f_mount(&fatfs, "", 1); // 挂载文件系统
    if (res != FR_OK)
    {
        DI_Console().WriteLine("f_mount error: " + std::to_string(res));
    }

    FIL file{};
    char const *filename = "example.txt";
    char const *str = "Hello, FATFS!";

    // 打开或创建文件
    res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK)
    {
        // 写入字符串到文件
        UINT bytesWritten;
        res = f_write(&file, str, strlen(str), &bytesWritten);
        if (res != FR_OK || bytesWritten != strlen(str))
        {
            DI_Console().WriteLine("write failed: " + std::to_string(res));
        }
        else
        {
            DI_Console().WriteLine("write " +
                                   std::to_string(bytesWritten) +
                                   " bytes to the file successfully");
        }

        // 关闭文件
        f_close(&file);
    }
    else
    {
        DI_Console().WriteLine("open file failed: " + std::to_string(res));
    }

    // 重新打开文件以读取
    res = f_open(&file, filename, FA_READ);
    if (res == FR_OK)
    {
        // 假设文件内容不会超过127个字符
        char buffer[128]{};
        UINT bytesRead;

        // 从文件读取内容
        res = f_read(&file, buffer, sizeof(buffer) - 1, &bytesRead);
        if (res != FR_OK)
        {
            DI_Console().WriteLine("read file failed: " + std::to_string(res));
        }
        else
        {
            DI_Console().WriteLine("read file successfully, have read " +
                                   std::to_string(bytesRead) +
                                   " bytes");

            DI_Console().WriteLine(buffer);
        }

        // 关闭文件
        f_close(&file);
    }
    else
    {
        DI_Console().WriteLine("open file failed: " + std::to_string(res));
    }

    // 卸载文件系统
    f_mount(NULL, "", 0);
}

uint8_t lwip_comm_init();
void lwip_periodic_handle();
void lwip_pkt_handle();
int DHCP_State();
uint8_t ethernet_chip_get_speed();

int main(void)
{
    DI_Initialize();

    DI_TaskManager().Create(
        []()
        {
            while (true)
            {
                DI_RedDigitalLed().Toggle();
                DI_Delayer().Delay(std::chrono::milliseconds{1000});
            }
        },
        512);

    DI_TaskManager().Create(
        []()
        {
            try
            {
                DI_Serial().Open(*DICreate_ISerialOptions());
                DI_Console().SetOutStream(base::RentedPtrFactory::Create(&DI_Serial()));
                SDRAM_Init();

                // TestLittleFs();
                // TestFatFs();
                while (lwip_comm_init())
                {
                    DI_Console().WriteLine("lwip_comm_init failed");
                    DI_Delayer().Delay(std::chrono::milliseconds{1000});
                }

                while (true)
                {
                    lwip_periodic_handle(); /* LWIP轮询任务 */
                    lwip_pkt_handle();
                }

                // while (true)
                // {
                //     DI_GreenDigitalLed().Toggle();
                //     // std::cout << eerom->ReadUInt64(0) << std::endl;
                //     // std::cout << lwip_localtime << std::endl;
                //     // PrintAddresses();
                //     // TestSDRAM();
                //     // DI_Console().WriteLine(DI_ClockSignalCollection().Get("hclk")->Frequency());
                //     DI_Delayer().Delay(std::chrono::seconds{1});
                // }

                // TestUniversalTimer1();
                // bsp::TestFlash();
                // TestExtiKey();
                // bsp::TestSerial();
                // bsp::TestKeyScanner();
                // bsp::TestIndependentWatchDog();
            }
            catch (std::exception const &e)
            {
                DI_Console().WriteError(e.what());
            }
            catch (...)
            {
                DI_Console().WriteError("发生了未知的异常");
            }
        },
        1024);

    DI_TaskManager().StartScheduler();
}
