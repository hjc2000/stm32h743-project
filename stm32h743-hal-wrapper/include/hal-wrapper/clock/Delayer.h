#pragma once
#include <bsp-interface/IDelayer.h>
#include <chrono>
#include <stdint.h>

namespace hal
{
	class Delayer : public bsp::IDelayer
	{
	private:
		Delayer() = default;
		Delayer(Delayer const &o) = delete;
		Delayer &operator=(Delayer const &o) = delete;

	public:
		static Delayer &Instance()
		{
			static Delayer o;
			return o;
		}

		virtual void Delay(std::chrono::microseconds microseconds) override;
		virtual void Delay(std::chrono::milliseconds milliseconds) override;
		virtual void Delay(std::chrono::seconds seconds) override;
	};
}