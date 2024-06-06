#include"UniversalTimerConfig.h"

using namespace hal;

hal::UniversalTimerConfig::UniversalTimerConfig(UniversalTimerConfig const &o)
{
	*this = o;
}

UniversalTimerConfig &hal::UniversalTimerConfig::operator=(UniversalTimerConfig const &o)
{
	return *this;
}

TIM_Base_InitTypeDef &hal::UniversalTimerConfig::Handle()
{
	return _handle;
}
