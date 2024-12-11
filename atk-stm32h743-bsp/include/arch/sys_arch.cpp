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
#include <bsp-interface/di/task.h>
#include <errno.h>

extern "C"
{
    void sys_init(void)
    {
    }

    u32_t sys_now(void)
    {
        return xTaskGetTickCount() * portTICK_PERIOD_MS;
    }

    u32_t sys_jiffies(void)
    {
        return xTaskGetTickCount();
    }

    void sys_arch_msleep(u32_t delay_ms)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{delay_ms});
    }
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    if (mbox == nullptr)
    {
        throw std::invalid_argument{"mbox 不能是空指针"};
    }

    LWIP_ASSERT("size > 0", size > 0);

    mbox->mbx = xQueueCreate((UBaseType_t)size, sizeof(void *));
    if (mbox->mbx == NULL)
    {
        SYS_STATS_INC(mbox.err);
        return ERR_MEM;
    }

    SYS_STATS_INC_USED(mbox);
    return ERR_OK;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    BaseType_t ret;
    if (mbox == nullptr)
    {
        throw std::invalid_argument{"mbox 不能是空指针"};
    }

    if (mbox->mbx == nullptr)
    {
        throw std::invalid_argument{"mbox->mbx 不能是空指针"};
    }

    ret = xQueueSendToBack(reinterpret_cast<QueueHandle_t>(mbox->mbx), &msg, portMAX_DELAY);
    LWIP_ASSERT("mbox post failed", ret == pdTRUE);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    BaseType_t ret;
    if (mbox == nullptr)
    {
        throw std::invalid_argument{"mbox 不能是空指针"};
    }

    if (mbox->mbx == nullptr)
    {
        throw std::invalid_argument{"mbox->mbx 不能是空指针"};
    }

    ret = xQueueSendToBack(reinterpret_cast<QueueHandle_t>(mbox->mbx), &msg, 0);
    if (ret == pdTRUE)
    {
        return ERR_OK;
    }
    else
    {
        LWIP_ASSERT("mbox trypost failed", ret == errQUEUE_FULL);
        SYS_STATS_INC(mbox.err);
        return ERR_MEM;
    }
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
    BaseType_t ret;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (mbox == nullptr)
    {
        throw std::invalid_argument{"mbox 不能是空指针"};
    }

    if (mbox->mbx == nullptr)
    {
        throw std::invalid_argument{"mbox->mbx 不能是空指针"};
    }

    ret = xQueueSendToBackFromISR(reinterpret_cast<QueueHandle_t>(mbox->mbx), &msg, &xHigherPriorityTaskWoken);
    if (ret == pdTRUE)
    {
        if (xHigherPriorityTaskWoken == pdTRUE)
        {
            return ERR_NEED_SCHED;
        }

        return ERR_OK;
    }
    else
    {
        LWIP_ASSERT("mbox trypost failed", ret == errQUEUE_FULL);
        SYS_STATS_INC(mbox.err);
        return ERR_MEM;
    }
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout_ms)
{
    BaseType_t ret;
    void *msg_dummy;
    if (mbox == nullptr)
    {
        throw std::invalid_argument{"mbox 不能是空指针"};
    }

    if (mbox->mbx == nullptr)
    {
        throw std::invalid_argument{"mbox->mbx 不能是空指针"};
    }

    if (!msg)
    {
        msg = &msg_dummy;
    }

    if (!timeout_ms)
    {
        /* wait infinite */
        ret = xQueueReceive(reinterpret_cast<QueueHandle_t>(mbox->mbx), &(*msg), portMAX_DELAY);
        LWIP_ASSERT("mbox fetch failed", ret == pdTRUE);
    }
    else
    {
        TickType_t timeout_ticks = timeout_ms / portTICK_PERIOD_MS;
        ret = xQueueReceive(reinterpret_cast<QueueHandle_t>(mbox->mbx), &(*msg), timeout_ticks);
        if (ret == errQUEUE_EMPTY)
        {
            /* timed out */
            *msg = NULL;
            return SYS_ARCH_TIMEOUT;
        }

        LWIP_ASSERT("mbox fetch failed", ret == pdTRUE);
    }

    /* Old versions of lwIP required us to return the time waited.
       This is not the case any more. Just returning != SYS_ARCH_TIMEOUT
       here is enough. */
    return 1;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    BaseType_t ret;
    void *msg_dummy;
    if (mbox == nullptr)
    {
        throw std::invalid_argument{"mbox 不能是空指针"};
    }

    if (mbox->mbx == nullptr)
    {
        throw std::invalid_argument{"mbox->mbx 不能是空指针"};
    }

    if (!msg)
    {
        msg = &msg_dummy;
    }

    ret = xQueueReceive(reinterpret_cast<QueueHandle_t>(mbox->mbx), &(*msg), 0);
    if (ret == errQUEUE_EMPTY)
    {
        *msg = NULL;
        return SYS_MBOX_EMPTY;
    }

    LWIP_ASSERT("mbox fetch failed", ret == pdTRUE);

    /* Old versions of lwIP required us to return the time waited.
       This is not the case any more. Just returning != SYS_ARCH_TIMEOUT
       here is enough. */
    return 1;
}

void sys_mbox_free(sys_mbox_t *mbox)
{
    if (mbox == nullptr)
    {
        throw std::invalid_argument{"mbox 不能是空指针"};
    }

    if (mbox->mbx == nullptr)
    {
        throw std::invalid_argument{"mbox->mbx 不能是空指针"};
    }

#if LWIP_FREERTOS_CHECK_QUEUE_EMPTY_ON_FREE
    {
        UBaseType_t msgs_waiting = uxQueueMessagesWaiting(mbox->mbx);
        LWIP_ASSERT("mbox quence not empty", msgs_waiting == 0);

        if (msgs_waiting != 0)
        {
            SYS_STATS_INC(mbox.err);
        }
    }
#endif

    vQueueDelete(reinterpret_cast<QueueHandle_t>(mbox->mbx));
    SYS_STATS_DEC(mbox.used);
}

sys_thread_t sys_thread_new(char const *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    TaskHandle_t rtos_task;
    BaseType_t ret;
    sys_thread_t lwip_thread;

    /* lwIP's lwip_thread_fn matches FreeRTOS' TaskFunction_t, so we can pass the
       thread function without adaption here. */
    ret = xTaskCreate(thread,
                      name,
                      (configSTACK_DEPTH_TYPE)stacksize,
                      arg,
                      1,
                      &rtos_task);

    LWIP_ASSERT("task creation failed", ret == pdTRUE);

    lwip_thread.thread_handle = rtos_task;
    return lwip_thread;
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
