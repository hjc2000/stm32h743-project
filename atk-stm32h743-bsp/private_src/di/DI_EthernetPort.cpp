#include <AtkApolloV1EthernetPort.h>
#include <AtkApolloV2EthernetPort.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/ethernet.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/ethernet/MutexEthernetPort.h>

namespace
{
	class Getter :
		public base::SingletonGetter<bsp::MutexEthernetPort>
	{
	public:
		std::unique_ptr<bsp::MutexEthernetPort> Create() override
		{
			return std::unique_ptr<bsp::MutexEthernetPort>{
				new bsp::MutexEthernetPort{
					&bsp::AtkApolloV1EthernetPort::Instance(),
					// &bsp::AtkApolloV2EthernetPort::Instance(),
				},
			};
		}

		void Lock() override
		{
			DI_DisableGlobalInterrupt();
		}

		void Unlock() override
		{
			DI_EnableGlobalInterrupt();
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
