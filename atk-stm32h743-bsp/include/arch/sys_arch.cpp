#include "FreeRTOS.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "semphr.h"
#include "task.h"
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/system_time.h>
#include <bsp-interface/di/task.h>
#include <errno.h>

extern "C"
{
    void sys_init(void)
    {
    }

    /// @brief 返回当前的系统时间。单位：ms.
    /// @param
    /// @return
    u32_t sys_now(void)
    {
        return static_cast<int64_t>(DI_SystemTime() * 1000);
    }

    /// @brief 返回系统计数器的值。这个值是要上电以来的计数。硬件 systick 时钟的频率太高了，
    /// 不考虑使用它。
    /// @param
    /// @return
    u32_t sys_jiffies(void)
    {
        return static_cast<int64_t>(DI_SystemTime() * 1000);
    }

    void sys_arch_msleep(u32_t delay_ms)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{delay_ms});
    }
}

#if LWIP_NETCONN_SEM_PER_THREAD
#if configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0

sys_sem_t *sys_arch_netconn_sem_get(void)
{
    void *ret;
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    LWIP_ASSERT("task != NULL", task != NULL);

    ret = pvTaskGetThreadLocalStoragePointer(task, 0);
    return ret;
}

void sys_arch_netconn_sem_alloc(void)
{
    void *ret;
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    LWIP_ASSERT("task != NULL", task != NULL);

    ret = pvTaskGetThreadLocalStoragePointer(task, 0);
    if (ret == NULL)
    {
        sys_sem_t *sem;
        err_t err;
        /* need to allocate the memory for this semaphore */
        sem = mem_malloc(sizeof(sys_sem_t));
        LWIP_ASSERT("sem != NULL", sem != NULL);
        err = sys_sem_new(sem, 0);
        LWIP_ASSERT("err == ERR_OK", err == ERR_OK);
        LWIP_ASSERT("sem invalid", sys_sem_valid(sem));
        vTaskSetThreadLocalStoragePointer(task, 0, sem);
    }
}

void sys_arch_netconn_sem_free(void)
{
    void *ret;
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    LWIP_ASSERT("task != NULL", task != NULL);

    ret = pvTaskGetThreadLocalStoragePointer(task, 0);
    if (ret != NULL)
    {
        sys_sem_t *sem = ret;
        sys_sem_free(sem);
        mem_free(sem);
        vTaskSetThreadLocalStoragePointer(task, 0, NULL);
    }
}

#else /* configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 */
#error LWIP_NETCONN_SEM_PER_THREAD needs configNUM_THREAD_LOCAL_STORAGE_POINTERS
#endif /* configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 */

#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#if LWIP_FREERTOS_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING

/** Flag the core lock held. A counter for recursive locks. */
static u8_t lwip_core_lock_count;
static TaskHandle_t lwip_core_lock_holder_thread;

void sys_lock_tcpip_core(void)
{
    sys_mutex_lock(&lock_tcpip_core);
    if (lwip_core_lock_count == 0)
    {
        lwip_core_lock_holder_thread = xTaskGetCurrentTaskHandle();
    }
    lwip_core_lock_count++;
}

void sys_unlock_tcpip_core(void)
{
    lwip_core_lock_count--;
    if (lwip_core_lock_count == 0)
    {
        lwip_core_lock_holder_thread = 0;
    }
    sys_mutex_unlock(&lock_tcpip_core);
}

#endif /* LWIP_TCPIP_CORE_LOCKING */

#if !NO_SYS
static TaskHandle_t lwip_tcpip_thread;
#endif

void sys_mark_tcpip_thread(void)
{
#if !NO_SYS
    lwip_tcpip_thread = xTaskGetCurrentTaskHandle();
#endif
}

void sys_check_core_locking(void)
{
    /* Embedded systems should check we are NOT in an interrupt context here */
    /* E.g. core Cortex-M3/M4 ports:
           configASSERT( ( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK ) == 0 );

       Instead, we use more generic FreeRTOS functions here, which should fail from ISR: */
    taskENTER_CRITICAL();
    taskEXIT_CRITICAL();

#if !NO_SYS
    if (lwip_tcpip_thread != 0)
    {
        TaskHandle_t current_thread = xTaskGetCurrentTaskHandle();

#if LWIP_TCPIP_CORE_LOCKING
        LWIP_ASSERT("Function called without core lock",
                    current_thread == lwip_core_lock_holder_thread && lwip_core_lock_count > 0);
#else  /* LWIP_TCPIP_CORE_LOCKING */
        LWIP_ASSERT("Function called from wrong thread", current_thread == lwip_tcpip_thread);
#endif /* LWIP_TCPIP_CORE_LOCKING */
    }
#endif /* !NO_SYS */
}

#endif /* LWIP_FREERTOS_CHECK_CORE_LOCKING*/
