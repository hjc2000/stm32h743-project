#pragma once
#include<bsp-interface/IIsrManager.h>

namespace hal
{
	class IsrManager :
		public bsp::IIsrManager
	{
	private:
		std::map<uint32_t, std::function<void()>> _handler_map;

	public:
		static IsrManager &Instance()
		{
			static IsrManager o;
			return o;
		}

		// 通过 IIsrManager 继承
		std::map<uint32_t, std::function<void()>> &HandlerMap() override;
		void DisableInterrupt(uint32_t irq) noexcept override;
		void EnableInterrupt(uint32_t irq) noexcept override;
	};
}