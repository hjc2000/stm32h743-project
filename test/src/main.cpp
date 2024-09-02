#include <atomic>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/led.h>
#include <bsp-interface/test/TestIndependentWatchDog.h>
#include <bsp-interface/test/TestKeyScanner.h>
#include <bsp-interface/test/TestSerial.h>
#include <bsp/bsp.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <task/Task.h>
#include <TestExtiKey.h>

int main(void)
{
    while (true)
    {
        try
        {
            BSP::Initialize();

            std::shared_ptr<task::Task> lvgl_init_task = task::Task::Create(
                []()
                {
                    // TestUniversalTimer1();
                    // TestFlash();
                    // TestExtiKey();
                    // bsp::TestSerial();
                    // bsp::TestKeyScanner();
                    bsp::TestIndependentWatchDog();
                },
                512);

            vTaskStartScheduler();
        }
        catch (std::exception const &e)
        {
            DI_RedDigitalLed().TurnOn();
        }
        catch (...)
        {
            DI_RedDigitalLed().TurnOn();
        }
    }
}
