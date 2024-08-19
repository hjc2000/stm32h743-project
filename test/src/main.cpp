#include <TestExtiKey.h>
#include <TestIndependentWatchDog.h>
#include <TestKeyScanner.h>
#include <atomic>
#include <bsp-interface/di.h>
#include <bsp/bsp.h>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <task/Task.h>

bool std_exception_occurred = false;
bool unknow_exception_occurred = false;

int main(void)
{
	while (true)
	{
		try
		{

			BSP::Initialize();

			auto lvgl_init_task_func = []()
			{
				// TestUniversalTimer1();
				// TestFlash();
				// TestKeyScanner();
			};
			std::shared_ptr<task::Task> lvgl_init_task = task::Task::Create(lvgl_init_task_func, 512);

			auto blink_red_led = []()
			{
				while (true)
				{
					// bsp::DI_Delayer().Delay(std::chrono::milliseconds{50});
					// BSP::RedDigitalLed().Toggle();
					TestGpio();
				}
			};
			std::shared_ptr<task::Task> blink_red_led_task = task::Task::Create(blink_red_led, 512);

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
