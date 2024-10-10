#pragma once
#include <base/define.h>

namespace hal
{
    class Cache
    {
    private:
        Cache() = delete;

    public:
        static_function void Enable();
        static_function void Disable();
    };
} // namespace hal
