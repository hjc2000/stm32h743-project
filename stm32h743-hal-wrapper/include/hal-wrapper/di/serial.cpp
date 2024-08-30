#include <base/container/Collection.h>
#include <bsp-interface/di/serial.h>
#include <hal-wrapper/peripheral/serial/Serial.h>

std::shared_ptr<bsp::ISerialOptions> DICreate_ISerialOptions()
{
    return std::shared_ptr<bsp::ISerialOptions>{new hal::SerialOptions{}};
}

/// @brief 主串口。
/// @return
bsp::ISerial &DI_Serial()
{
    return hal::Serial::Instance();
}

class Collection
{
public:
    Collection()
    {
        Add(&hal::Serial::Instance());
    }

    base::Collection<std::string, bsp::ISerial *> _collection{};

    void Add(bsp::ISerial *o)
    {
        _collection.Put(o->Name(), o);
    }
};

/// @brief 包含所有可用串口的集合。
/// @return
base::ICollection<std::string, bsp::ISerial *> const &DI_SerialCollection()
{
    static Collection o;
    return o._collection;
}
