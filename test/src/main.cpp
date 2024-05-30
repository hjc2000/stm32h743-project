#include<atomic>
#include<bsp/bsp.h>
#include<memory>
#include<stdexcept>
#include<stdint.h>
#include<task/Task.h>
#include<TestKeyScanner.h>

bool std_exception_occurred = false;
bool unknow_exception_occurred = false;

int main(void)
{
	while (true)
	{
		try
		{

			BSP::Initialize();
			std::shared_ptr<task::Task> lvgl_init_task = task::Task::Create([]()
			{
				TestKeyScanner();
			}, 512);
			vTaskStartScheduler();
		}
		catch (std::exception &e)
		{
			std_exception_occurred = true;
		}
		catch (...)
		{
			unknow_exception_occurred = true;
		}
	}
}
