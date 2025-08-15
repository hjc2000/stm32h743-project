#include "base/embedded/heap/heap.h"
#include "base/define.h"
#include "base/embedded/heap/Heap4.h"
#include "base/embedded/heap/IHeap.h"
#include <cstddef>
#include <stdlib.h>
#include <string.h>

namespace
{
	uint8_t _buffer[static_cast<size_t>(1024) * 200];

	base::heap::Heap4 &Heap4Instance()
	{
		static base::heap::Heap4 heap4{_buffer, sizeof(_buffer)};
		return heap4;
	}

} // namespace

PREINIT(Heap4Instance)

/// @brief 获取主堆。
/// @return
base::heap::IHeap &base::heap::Heap()
{
	return Heap4Instance();
}
