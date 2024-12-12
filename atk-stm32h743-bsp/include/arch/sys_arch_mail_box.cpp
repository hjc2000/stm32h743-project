#include "FreeRTOS.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "semphr.h"
#include "task.h"
#include <arch/sys_arch.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/system_time.h>
#include <bsp-interface/di/task.h>
#include <errno.h>
#include <FreeRTOS.h>
#include <lwip/sys.h>
#include <queue.h>
#include <stdexcept>

extern "C"
{
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
}
