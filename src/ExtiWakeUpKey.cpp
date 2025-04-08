#include "ExtiWakeUpKey.h"

using namespace bsp;

ExtiWakeUpKey::ExtiWakeUpKey()
{
	_pin = DI_GpioPinCollection().Get("PH3");
	_pin->OpenAsInputMode(bsp::IGpioPinPullMode::PullUp,
						  bsp::IGpioPinTriggerEdge::RisingEdge);

	_pin->RegisterInterruptCallback(
		[&]()
		{
			// 这是在中断函数中，禁止使用 Delayer 进行延时。
			DI_SysTick().Delay(std::chrono::milliseconds{20});
			_is_pressed = _pin->ReadPin();
		});
}

bsp::ExtiWakeUpKey &bsp::ExtiWakeUpKey::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<ExtiWakeUpKey>
	{
	public:
		std::unique_ptr<ExtiWakeUpKey> Create() override
		{
			return std::unique_ptr<ExtiWakeUpKey>{new ExtiWakeUpKey{}};
		}
	};

	Getter g;
	return g.Instance();
}
