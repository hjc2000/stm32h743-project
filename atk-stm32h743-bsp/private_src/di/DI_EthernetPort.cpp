#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <bsp-interface/ethernet/MutexEthernetPort.h>
#include <EthernetPort.h>

namespace
{
	class Getter :
		public bsp::TaskSingletonGetter<bsp::MutexEthernetPort>
	{
	public:
		std::unique_ptr<bsp::MutexEthernetPort> Create() override
		{
			return std::unique_ptr<bsp::MutexEthernetPort>{
				new bsp::MutexEthernetPort{
					&bsp::EthernetPort::Instance(),
				},
			};
		}
	};

} // namespace

/// @brief 以太网端口。
/// @return
bsp::IEthernetPort &DI_EthernetPort()
{
	Getter g;
	return g.Instance();
}
