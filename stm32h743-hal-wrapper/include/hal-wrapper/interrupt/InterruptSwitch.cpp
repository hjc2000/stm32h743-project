#include "InterruptSwitch.h"
#include <base/Initializer.h>

static base::Initializer _initializer{
	[]()
	{
		hal::InterruptSwitch::Instance();
	}};
