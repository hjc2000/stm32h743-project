#pragma once

namespace hal
{
	class Cache
	{
	private:
		Cache() = delete;

	public:
		static void Enable();
		static void Disable();
	};
}