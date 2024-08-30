#include "DmaOptions.h"
#include <base/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>

std::map<std::string, uint32_t> const &bsp::DmaOptions::RequestMap()
{
    class Getter : public base::SingletonGetter<std::map<std::string, uint32_t>>
    {
    public:
        std::unique_ptr<std::map<std::string, uint32_t>> Create() override
        {
            return std::unique_ptr<std::map<std::string, uint32_t>>{
                new std::map<std::string, uint32_t>{
                    {"usart1_rx", DMA_REQUEST_USART1_RX},
                    {"usart1_tx", DMA_REQUEST_USART1_TX},
                },
            };
        }

        void Lock() override
        {
            DI_InterruptSwitch().DisableGlobalInterrupt();
        }

        void Unlock() override
        {
            DI_InterruptSwitch().EnableGlobalInterrupt();
        }
    };

    Getter o;
    return o.Instance();
}

bsp::DmaOptions::DmaOptions()
{
    _init_type_def.Mode = DMA_NORMAL;
    _init_type_def.FIFOMode = DMA_FIFOMODE_DISABLE;
    _init_type_def.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    _init_type_def.MemBurst = DMA_MBURST_SINGLE;
    _init_type_def.PeriphBurst = DMA_PBURST_SINGLE;
}

bsp::DmaOptions::operator DMA_InitTypeDef() const
{
    return _init_type_def;
}

bsp::IDmaOptions_Direction bsp::DmaOptions::Direction() const
{
    switch (_init_type_def.Direction)
    {
    case DMA_PERIPH_TO_MEMORY:
        {
            return IDmaOptions_Direction::PeripheralToMemory;
        }
    case DMA_MEMORY_TO_PERIPH:
        {
            return IDmaOptions_Direction::MemoryToPeripheral;
        }
    case DMA_MEMORY_TO_MEMORY:
        {
            return IDmaOptions_Direction::MomoryToMemory;
        }
    default:
        {
            throw std::invalid_argument{"非法方向"};
        }
    }
}

void bsp::DmaOptions::SetDirection(IDmaOptions_Direction value)
{
    switch (value)
    {
    case IDmaOptions_Direction::PeripheralToMemory:
        {
            _init_type_def.Direction = DMA_PERIPH_TO_MEMORY;
            break;
        }
    case IDmaOptions_Direction::MemoryToPeripheral:
        {
            _init_type_def.Direction = DMA_MEMORY_TO_PERIPH;
            break;
        }
    case IDmaOptions_Direction::MomoryToMemory:
        {
            _init_type_def.Direction = DMA_MEMORY_TO_MEMORY;
            break;
        }
    default:
        {
            throw std::invalid_argument{"非法方向"};
        }
    }
}

bool bsp::DmaOptions::PeripheralIncrement() const
{
    if (_init_type_def.PeriphInc == DMA_PINC_ENABLE)
    {
        return true;
    }

    return false;
}

void bsp::DmaOptions::SetPeripheralIncrement(bool value)
{
    _init_type_def.PeriphInc = value ? DMA_PINC_ENABLE : DMA_PINC_DISABLE;
}

bool bsp::DmaOptions::MemoryIncrement() const
{
    if (_init_type_def.MemInc == DMA_MINC_ENABLE)
    {
        return true;
    }

    return false;
}

void bsp::DmaOptions::SetMemoryIncrement(bool value)
{
    _init_type_def.MemInc = value ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;
}

int bsp::DmaOptions::PeripheralDataAlignment() const
{
    switch (_init_type_def.PeriphDataAlignment)
    {
    case DMA_PDATAALIGN_BYTE:
        {
            return 1;
        }
    case DMA_PDATAALIGN_HALFWORD:
        {
            return 2;
        }
    case DMA_PDATAALIGN_WORD:
        {
            return 4;
        }
    default:
        {
            throw std::invalid_argument{"非法对齐方式"};
        }
    }
}

void bsp::DmaOptions::SetPeripheralDataAlignment(int value)
{
    switch (value)
    {
    case 1:
        {
            _init_type_def.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            break;
        }
    case 2:
        {
            _init_type_def.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
            break;
        }
    case 4:
        {
            _init_type_def.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
            break;
        }
    default:
        {
            throw std::invalid_argument{"非法对齐方式"};
        }
    }
}

int bsp::DmaOptions::MemoryDataAlignment() const
{
    switch (_init_type_def.MemDataAlignment)
    {
    case DMA_MDATAALIGN_BYTE:
        {
            return 1;
        }
    case DMA_MDATAALIGN_HALFWORD:
        {
            return 2;
        }
    case DMA_MDATAALIGN_WORD:
        {
            return 4;
        }
    default:
        {
            throw std::invalid_argument{"非法对齐方式"};
        }
    }
}

void bsp::DmaOptions::SetMemoryDataAlignment(int value)
{
    switch (value)
    {
    case 1:
        {
            _init_type_def.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            break;
        }
    case 2:
        {
            _init_type_def.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
            break;
        }
    case 4:
        {
            _init_type_def.MemDataAlignment = DMA_MDATAALIGN_WORD;
            break;
        }
    default:
        {
            throw std::invalid_argument{"非法对齐方式"};
        }
    }
}

bsp::IDmaOptions_Priority bsp::DmaOptions::Priority() const
{
    switch (_init_type_def.Priority)
    {
    case DMA_PRIORITY_LOW:
        {
            return IDmaOptions_Priority::Low;
        }
    case DMA_PRIORITY_MEDIUM:
        {
            return IDmaOptions_Priority::Medium;
        }
    case DMA_PRIORITY_HIGH:
        {
            return IDmaOptions_Priority::High;
        }
    case DMA_PRIORITY_VERY_HIGH:
        {
            return IDmaOptions_Priority::VeryHigh;
        }
    default:
        {
            throw std::invalid_argument{"非法优先级"};
        }
    }
}

void bsp::DmaOptions::SetPriority(IDmaOptions_Priority value)
{
    switch (value)
    {
    case IDmaOptions_Priority::Low:
        {
            _init_type_def.Priority = DMA_PRIORITY_LOW;
            break;
        }
    case IDmaOptions_Priority::Medium:
        {
            _init_type_def.Priority = DMA_PRIORITY_MEDIUM;
            break;
        }
    case IDmaOptions_Priority::High:
        {
            _init_type_def.Priority = DMA_PRIORITY_HIGH;
            break;
        }
    case IDmaOptions_Priority::VeryHigh:
        {
            _init_type_def.Priority = DMA_PRIORITY_VERY_HIGH;
            break;
        }
    default:
        {
            throw std::invalid_argument{"非法优先级"};
        }
    }
}

std::string bsp::DmaOptions::Request() const
{
    return _request;
}

void bsp::DmaOptions::SetRequest(std::string value)
{
    _request = value;
    auto it = RequestMap().find(_request);
    if (it == RequestMap().end())
    {
        throw std::runtime_error{"不支持此请求"};
    }

    _init_type_def.Request = it->second;
}
