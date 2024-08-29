#include <atomic>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/led.h>
#include <bsp-interface/test/TestSerial.h>
#include <bsp/bsp.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <task/Task.h>
#include <TestExtiKey.h>

bool std_exception_occurred = false;
bool unknow_exception_occurred = false;

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
                    // TestKeyScanner();

                    bsp::TestSerial();

                    // while (true)
                    // {
                    //     DI_GreenDigitalLed().Toggle();
                    //     DI_RedDigitalLed().Toggle();
                    //     DI_Delayer().Delay(std::chrono::seconds{1});
                    // }
                },
                512);

            vTaskStartScheduler();
        }
        catch (std::exception const &e)
        {
            std_exception_occurred = true;
        }
        catch (...)
        {
            unknow_exception_occurred = true;
        }
    }
}
