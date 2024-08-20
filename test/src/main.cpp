#include <atomic>
#include <bsp-interface/di.h>
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
