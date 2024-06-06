#include"UniversalTimer1.h"

TIM_HandleTypeDef &hal::UniversalTimer1::Handle()
{
	return _handle;
}
