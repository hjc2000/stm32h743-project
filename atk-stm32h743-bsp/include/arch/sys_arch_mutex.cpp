#include <arch/sys_arch.h>
#include <bsp-interface/di/task.h>
#include <lwip/err.h>

namespace
{
    class MutexHandle
    {
    public:
        std::shared_ptr<bsp::IMutex> _mutex = DICreate_Mutex();
    };

} // namespace

extern "C"
{
    /* Create a new mutex*/
    err_t sys_mutex_new(sys_mutex_t *mutex)
    {
        if (mutex == nullptr)
        {
            throw std::invalid_argument{"mutex 不能是空指针"};
        }

        if (mutex->mut != nullptr)
        {
            delete reinterpret_cast<MutexHandle *>(mutex->mut);
        }

        mutex->mut = new MutexHandle{};
        return ERR_OK;
    }

    void sys_mutex_lock(sys_mutex_t *mutex)
    {
        if (mutex == nullptr)
        {
            throw std::invalid_argument{"mutex 不能是空指针"};
        }

        if (mutex->mut == nullptr)
        {
            throw std::invalid_argument{"mutex->mut 不能是空指针"};
        }

        reinterpret_cast<MutexHandle *>(mutex->mut)->_mutex->Lock();
    }

    void sys_mutex_unlock(sys_mutex_t *mutex)
    {
        if (mutex == nullptr)
        {
            throw std::invalid_argument{"mutex 不能是空指针"};
        }

        if (mutex->mut == nullptr)
        {
            throw std::invalid_argument{"mutex->mut 不能是空指针"};
        }

        reinterpret_cast<MutexHandle *>(mutex->mut)->_mutex->Unlock();
    }

    void sys_mutex_free(sys_mutex_t *mutex)
    {
        if (mutex == nullptr)
        {
            return;
        }

        if (mutex->mut == nullptr)
        {
            return;
        }

        delete reinterpret_cast<MutexHandle *>(mutex->mut);
        mutex->mut = nullptr;
    }
}
