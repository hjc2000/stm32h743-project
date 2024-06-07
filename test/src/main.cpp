#include<atomic>
#include<bsp/bsp.h>
#include<memory>
#include<stdexcept>
#include<stdint.h>
#include<task/Task.h>
#include<TestExtiKey.h>
#include<TestIndependentWatchDog.h>
#include<TestKeyScanner.h>
#include<TestSerial.h>

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
				TestUniversalTimer1();
			}, 512);
			vTaskStartScheduler();
		}
		catch (std::exception &e)
		{
			BSP::RedDigitalLed().TurnOff();
			std_exception_occurred = true;
		}
		catch (...)
		{
			BSP::RedDigitalLed().TurnOff();
			unknow_exception_occurred = true;
		}
	}
}
