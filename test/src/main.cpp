#include <atomic>
#include <base/RentedPtrFactory.h>
#include <base/string/ToHexString.h>
#include <bsp-interface/di/clock.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
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
                    TestLittleFs();
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
