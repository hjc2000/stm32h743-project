#include <bsp-interface/di/system_time.h>
#include <SysTickClock.h>

/// @brief 当前的系统时间。
/// @note 这里返回的单位是秒。实现者可以有不同的时间精度，因为 base::Seconds 是一个分数，
/// 例如实现者的精度如果是毫秒，则可以每毫秒让 DI_SystemTime 递增 1/1000 秒。
/// @return
base::Seconds DI_SystemTime()
{
    return bsp::SysTickClock::Instance().SystemTime();
}
