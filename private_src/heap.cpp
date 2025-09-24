#include "base/embedded/heap/heap.h"
#include "base/embedded/heap/Heap4.h"
#include "base/embedded/heap/IHeap.h"
#include "base/SingletonProvider.h"
#include <cstdint>

namespace
{
	struct HeapContext
	{
		uint8_t *_buffer = reinterpret_cast<uint8_t *>(0x24000000);
		base::heap::Heap4 heap4{_buffer, sizeof(_buffer)};
	};

	base::SingletonProvider<HeapContext> _heap_context_instance_provider;

} // namespace

///
/// @brief 获取主堆。
/// @return
///
base::heap::IHeap &base::heap::Heap()
{
	return _heap_context_instance_provider.Instance().heap4;
}
