#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include <arch/sys_arch.h>
#include <bsp-interface/di/task.h>
#include <lwip/err.h>
#include <lwip/sys.h>

namespace
{
    /// @brief 信号量句柄。
    /// @note 因为 bsp-interface 中定义的 API 使用的是共享指针，与 lwip 的 C 不兼容，
    /// 所以需要使用一个句柄类包装一下。
    class SemaphoreHandle
    {
    public:
        std::shared_ptr<bsp::IBinarySemaphore> _semaphore = DICreate_BinarySemaphore();
    };
} // namespace

extern "C"
{
    /// @brief 构造一个信号量对象。
    /// @param sem
    /// @param initial_count
    /// @return
    err_t sys_sem_new(sys_sem_t *sem, u8_t initial_count)
    {
        if (sem == nullptr)
        {
            throw std::invalid_argument{"sem 不能是空指针"};
        }

        sem->sem = new SemaphoreHandle{};
        if (initial_count != 0)
        {
            reinterpret_cast<SemaphoreHandle *>(sem->sem)
                ->_semaphore
                ->Release();
        }

        return ERR_OK;
    }

    /// @brief 释放信号量。
    /// @param sem
    void sys_sem_signal(sys_sem_t *sem)
    {
        if (sem == nullptr)
        {
            throw std::invalid_argument{"sem 不能是空指针"};
        }

        if (sem->sem == nullptr)
        {
            throw std::invalid_argument{"sem->sem 不能是空指针"};
        }

        reinterpret_cast<SemaphoreHandle *>(sem->sem)
            ->_semaphore
            ->Release();
    }

    /// @brief
    /// @param sem
    ///
    /// @param timeout_ms 超时时间。单位：ms.
    /// @note 传入大于 0 的数表示要使用超时模式。传入 0 表示无限等待。
    ///
    /// @return 正常结束等待返回 1，超时时返回 SYS_ARCH_TIMEOUT.
    u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout_ms)
    {
        if (sem == nullptr)
        {
            throw std::invalid_argument{"sem 不能是空指针"};
        }

        if (sem->sem == nullptr)
        {
            throw std::invalid_argument{"sem->sem 不能是空指针"};
        }

        reinterpret_cast<SemaphoreHandle *>(sem->sem)
            ->_semaphore
            ->TryAcquire(std::chrono::milliseconds{timeout_ms});

        return 1;
    }

    /// @brief 释放信号量对象。执行析构，释放内存。
    /// @param sem
    void sys_sem_free(sys_sem_t *sem)
    {
        if (sem == nullptr)
        {
            return;
        }

        if (sem->sem == nullptr)
        {
            return;
        }

        delete reinterpret_cast<SemaphoreHandle *>(sem->sem);
        sem->sem = nullptr;
    }
}
