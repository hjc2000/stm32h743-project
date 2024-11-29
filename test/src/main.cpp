#include <atomic>
#include <base/RentedPtrFactory.h>
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
#include <littlefs/LfsFlashPort.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <TestExtiKey.h>

inline void TestSdram()
{
    SDRAM_Init(); // 初始化SDRAM
    uint32_t volatile *buffer = reinterpret_cast<uint32_t volatile *>(0XC0000000);
    for (uint8_t i = 0; i < 10; i++)
    {
        buffer[i] = i;
    }

    for (uint8_t i = 0; i < 10; i++)
    {
        DI_Console().WriteLine(std::to_string(static_cast<int>(buffer[i])));
    }
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
                    // Lfs::TestLittleFs();
                    TestSdram();
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
                512);

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
