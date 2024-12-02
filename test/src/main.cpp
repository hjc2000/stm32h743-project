#include <atomic>
#include <base/RentedPtrFactory.h>
#include <base/string/ToHexString.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/led.h>
#include <bsp-interface/di/task.h>
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

inline void TestSdram()
{
    while (true)
    {
        using element_type = uint8_t;
        int const one_MB = 1024 * 1024;
        int const count = 2 * one_MB / sizeof(element_type);
        element_type *buffer = reinterpret_cast<element_type *>(0XC0000000 + one_MB * 16);

        for (uint32_t i = 0; i < count; i++)
        {
            element_type value = static_cast<element_type>(i);
            buffer[i] = value;
        }

        for (uint32_t i = 0; i < count; i++)
        {
            element_type value = static_cast<element_type>(i);
            if (buffer[i] != value)
            {
                DI_Console().WriteLine("sdram error, value = " + std::to_string(value) +
                                       ", buffer[i] = " + std::to_string(buffer[i]));
                return;
            }
        }

        DI_Console().WriteLine("sdram no error");
    }
}

BYTE work[FF_MAX_SS * 2]; // 工作缓冲区

inline void TestFatFs()
{
    FATFS fatfs{};
    FRESULT res{};

    res = f_mount(&fatfs, "", 0); // 挂载文件系统
    if (res != FR_OK)
    {
        // 处理错误
        DI_Console().WriteLine("f_mount error: " + std::to_string(res));
    }
    else
    {
        DI_Console().WriteLine("f_mount successfully: " + std::to_string(res));
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
        // 处理错误
        DI_Console().WriteLine("f_mkfs error: " + std::to_string(res));
    }
    else
    {
        DI_Console().WriteLine("f_mkfs successfully: " + std::to_string(res));
    }

    // 重新挂载文件系统
    res = f_mount(&fatfs, "", 1); // 挂载文件系统
    if (res != FR_OK)
    {
        // 处理错误
        DI_Console().WriteLine("f_mount error: " + std::to_string(res));
    }
    else
    {
        DI_Console().WriteLine("f_mount successfully: " + std::to_string(res));
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
            DI_Console().WriteLine("write failed:" + std::to_string(res));
        }
        else
        {
            DI_Console().WriteLine("write successfully " + std::to_string(bytesWritten) + "bytes");
        }

        // 关闭文件
        f_close(&file);
    }
    else
    {
        DI_Console().WriteLine("open failed:" + std::to_string(res));
    }

    // 重新打开文件以读取
    res = f_open(&file, filename, FA_READ);
    if (res == FR_OK)
    {
        char buffer[128] = {0}; // 假设文件内容不会超过127个字符
        UINT bytesRead;

        // 从文件读取内容
        res = f_read(&file, buffer, sizeof(buffer) - 1, &bytesRead);
        if (res != FR_OK)
        {
            printf("读取文件失败: %d\n", res);
        }
        else
        {
            printf("成功读取 %u 字节: %s\n", bytesRead, buffer);
        }

        // 关闭文件
        f_close(&file);
    }
    else
    {
        printf("打开文件失败: %d\n", res);
    }

    // 卸载文件系统
    f_mount(NULL, "", 0);
}

int main(void)
{
    while (true)
    {
        try
        {
            BSP::Initialize();

            DI_TaskManager().Create(
                []()
                {
                    DI_Serial().Open(*DICreate_ISerialOptions());
                    DI_Console().SetOutStream(base::RentedPtrFactory::Create(&DI_Serial()));
                    SDRAM_Init();
                    // Lfs::TestLittleFs();
                    // TestSdram();
                    TestFatFs();
                    while (true)
                    {
                        DI_GreenDigitalLed().Toggle();
                        DI_Delayer().Delay(std::chrono::seconds{1});
                    }

                    // TestUniversalTimer1();
                    // bsp::TestFlash();
                    // TestExtiKey();
                    // bsp::TestSerial();
                    // bsp::TestKeyScanner();
                    // bsp::TestIndependentWatchDog();
                },
                1024);

            DI_TaskManager().StartScheduler();
        }
        catch (std::exception const &e)
        {
            DI_Console().WriteError(e.what());
        }
        catch (...)
        {
            DI_Console().WriteError("发生了未知的异常");
        }
    }
}
