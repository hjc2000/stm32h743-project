#include"DmaConfig.h"

using namespace hal;

DmaConfig::DmaConfig(DMA_InitTypeDef const &o)
{
	*this = o;
}

DmaConfig &DmaConfig::operator=(DMA_InitTypeDef const &o)
{
	_request = static_cast<decltype(_request)>(o.Request);
	_direction = static_cast<decltype(_direction)>(o.Direction);
	_peripheral_address_increase = static_cast<decltype(_peripheral_address_increase)>(o.PeriphInc);
	_memory_address_increase = static_cast<decltype(_memory_address_increase)>(o.MemInc);
	_peripheral_data_alignment = static_cast<decltype(_peripheral_data_alignment)>(o.PeriphDataAlignment);
	_memory_data_alignment = static_cast<decltype(_memory_data_alignment)>(o.MemDataAlignment);
	_mode = static_cast<decltype(_mode)>(o.Mode);
	_priority = static_cast<decltype(_priority)>(o.Priority);
	_fifo_mode = static_cast<decltype(_fifo_mode)>(o.FIFOMode);
	_fifo_threshold = static_cast<decltype(_fifo_threshold)>(o.FIFOThreshold);
	_memory_burst = static_cast<decltype(_memory_burst)>(o.MemBurst);
	_peripheral_burst = static_cast<decltype(_peripheral_burst)>(o.PeriphBurst);
	return *this;
}

hal::DmaConfig::operator DMA_InitTypeDef() const
{
	DMA_InitTypeDef o;
	o.Request = static_cast<uint32_t>(_request);
	o.Direction = static_cast<uint32_t>(_direction);
	o.PeriphInc = static_cast<uint32_t>(_peripheral_address_increase);
	o.MemInc = static_cast<uint32_t>(_memory_address_increase);
	o.PeriphDataAlignment = static_cast<uint32_t>(_peripheral_data_alignment);
	o.MemDataAlignment = static_cast<uint32_t>(_memory_data_alignment);
	o.Mode = static_cast<uint32_t>(_mode);
	o.Priority = static_cast<uint32_t>(_priority);
	o.FIFOMode = static_cast<uint32_t>(_fifo_mode);
	o.FIFOThreshold = static_cast<uint32_t>(_fifo_threshold);
	o.MemBurst = static_cast<uint32_t>(_memory_burst);
	o.PeriphBurst = static_cast<uint32_t>(_peripheral_burst);
	return o;
}
