#include <arch/sys_arch.h>
#include <bsp-interface/di/task.h>
#include <lwip/sys.h>

extern "C"
{
    /// @brief 创建新线程。
    /// @param name 线程名称。
    /// @param thread 线程函数。
    /// @param arg 线程参数。调用线程函数的时候要传递给线程函数。
    /// @param stacksize 线程栈大小。单位：字。32 位的计算机 1 个字是 4 字节。
    /// @param prio 线程优先级。
    ///
    /// @return 返回线程句柄。
    /// @note 本函数的实现并不会返回有效的句柄，线程由 DI_TaskManager 自动管理，
    /// 线程函数返回的时候线程的资源会被自动回收。
    sys_thread_t sys_thread_new(char const *name,
                                lwip_thread_fn thread,
                                void *arg,
                                int stacksize,
                                int prio)
    {
        DI_TaskManager().Create(
            [thread, arg]()
            {
                thread(arg);
            },
            stacksize);

        sys_thread_t ret{};
        return ret;
    }
}
