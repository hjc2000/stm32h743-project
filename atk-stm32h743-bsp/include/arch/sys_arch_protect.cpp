#include <arch/sys_arch.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>

namespace
{
    class LwipSysArch
    {
    private:
        LwipSysArch() = default;

        std::shared_ptr<bsp::IMutex> _arch_protect_mutex = DICreate_Mutex();

    public:
        static_function LwipSysArch &Instance()
        {
            class Getter :
                public base::SingletonGetter<LwipSysArch>
            {
            public:
                std::unique_ptr<LwipSysArch> Create() override
                {
                    return std::unique_ptr<LwipSysArch>{new LwipSysArch{}};
                }

                void Lock() override
                {
                    DI_DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_EnableGlobalInterrupt();
                }
            };

            Getter o;
            return o.Instance();
        }

        /// @brief 用于实现 sys_arch_protect
        void ArchProtect()
        {
            _arch_protect_mutex->Lock();
        }

        /// @brief 用于实现 sys_arch_unprotect
        void ArchUnprotect()
        {
            _arch_protect_mutex->Unlock();
        }
    };

} // namespace

extern "C"
{
    sys_prot_t sys_arch_protect(void)
    {
        LwipSysArch::Instance().ArchProtect();
        return 1;
    }

    void sys_arch_unprotect(sys_prot_t pval)
    {
        LwipSysArch::Instance().ArchUnprotect();
    }
}
