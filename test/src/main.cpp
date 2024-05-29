#include<atk-stm32h743-bsp/bsp.h>
#include<atomic>
#include<memory>
#include<stdexcept>
#include<stdint.h>
#include<task/Task.h>

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
				while (true)
				{
					BSP::RedDigitalLed().Toggle();
					BSP::GreenDigitalLed().Toggle();
					BSP::Delayer().Delay(std::chrono::seconds { 1 });
				}
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
