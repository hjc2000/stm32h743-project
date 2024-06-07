#include"UniversalTimerConfig.h"

using namespace hal;

TIM_Base_InitTypeDef &hal::UniversalTimerConfig::Handle()
{
	return _handle;
}
