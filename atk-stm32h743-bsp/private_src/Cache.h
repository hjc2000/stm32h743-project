#pragma once

namespace bsp
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