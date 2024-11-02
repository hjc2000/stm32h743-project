#include <atomic>
#include <base/RentedPtrFactory.h>
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/led.h>
#include <bsp-interface/di/task.h>
#include <bsp-interface/test/TestFlash.h>
#include <bsp-interface/test/TestIndependentWatchDog.h>
#include <bsp-interface/test/TestKeyScanner.h>
#include <bsp-interface/test/TestSerial.h>
#include <bsp/bsp.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <TestExtiKey.h>

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
                    while (true)
                    {
                        bsp::Console::Instance().WriteLine("6666666666");
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
        }
        catch (...)
        {
        }
    }
}
