#include <sys/system_properties.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/time.h>

typedef int32_t (*perfLockAcquire)(int, int, int[], int);
typedef int32_t (*perfLockRelease)(int);

typedef enum {
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_0     = 0x40800000,
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_1     = 0x40800010,
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_2     = 0x40800020,
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_3     = 0x40800030,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_0  = 0x40800100,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_1  = 0x40800110,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_2  = 0x40800120,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_3  = 0x40800130,

    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_0     = 0x40804000,
    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_1     = 0x40804010,
    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_2     = 0x40804020,
    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_3     = 0x40804030,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_0  = 0x40804100,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_1  = 0x40804110,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_2  = 0x40804120,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_3  = 0x40804130,

    MPCTLV3_MIN_ONLINE_CPU_CLUSTER_BIG      = 0x41000000,
    MPCTLV3_MIN_ONLINE_CPU_CLUSTER_LITTLE   = 0x41000100,
    MPCTLV3_MAX_ONLINE_CPU_CLUSTER_BIG      = 0x41004000,
    MPCTLV3_MAX_ONLINE_CPU_CLUSTER_LITTLE   = 0x41004100,

    MPCTLV3_ALL_CPUS_PWR_CLPS_DIS           = 0x40400000
} perf_lock_params;

static int32_t perfLockParamsFullUp[] = {
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_0     ,  0xFFF,
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_1     ,  0xFFF,
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_2     ,  0xFFF,
    MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_3     ,  0xFFF,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_0  ,  0xFFF,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_1  ,  0xFFF,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_2  ,  0xFFF,
    MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_3  ,  0xFFF,
    
    // MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_0     ,  0xFFF,
    // MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_1     ,  0xFFF,
    // MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_2     ,  0xFFF,
    // MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_3     ,  0xFFF,
    // MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_0  ,  0xFFF,
    // MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_1  ,  0xFFF,
    // MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_2  ,  0xFFF,
    // MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_3  ,  0xFFF,
    
    MPCTLV3_MIN_ONLINE_CPU_CLUSTER_BIG      ,  0x4,
    MPCTLV3_MIN_ONLINE_CPU_CLUSTER_LITTLE   ,  0x4,
    MPCTLV3_MAX_ONLINE_CPU_CLUSTER_BIG      ,  0x4,
    MPCTLV3_MAX_ONLINE_CPU_CLUSTER_LITTLE   ,  0x4,

    MPCTLV3_ALL_CPUS_PWR_CLPS_DIS           ,  0x1,
};

static int32_t perfLockParamsFullUp2[] = {
    // MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_0     ,  0xFFF,
    // MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_1     ,  0xFFF,
    // MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_2     ,  0xFFF,
    // MPCTLV3_MIN_FREQ_CLUSTER_BIG_CORE_3     ,  0xFFF,
    // MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_0  ,  0xFFF,
    // MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_1  ,  0xFFF,
    // MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_2  ,  0xFFF,
    // MPCTLV3_MIN_FREQ_CLUSTER_LITTLE_CORE_3  ,  0xFFF,
    
    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_0     ,  0xFFF,
    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_1     ,  0xFFF,
    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_2     ,  0xFFF,
    MPCTLV3_MAX_FREQ_CLUSTER_BIG_CORE_3     ,  0xFFF,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_0  ,  0xFFF,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_1  ,  0xFFF,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_2  ,  0xFFF,
    MPCTLV3_MAX_FREQ_CLUSTER_LITTLE_CORE_3  ,  0xFFF,
    
    MPCTLV3_MIN_ONLINE_CPU_CLUSTER_BIG      ,  0x4,
    MPCTLV3_MIN_ONLINE_CPU_CLUSTER_LITTLE   ,  0x4,
    MPCTLV3_MAX_ONLINE_CPU_CLUSTER_BIG      ,  0x4,
    MPCTLV3_MAX_ONLINE_CPU_CLUSTER_LITTLE   ,  0x4,

    MPCTLV3_ALL_CPUS_PWR_CLPS_DIS           ,  0x1,
};

#define DEFAULT_PERF_LOCK_TIMEOUT_MS 1000
#define PERF_LIBRARY "ro.vendor.extension_library"

class set_cpu
{
public:
    set_cpu(int ms = DEFAULT_PERF_LOCK_TIMEOUT_MS, const char* name = PERF_LIBRARY)
    {
        char value[PROP_VALUE_MAX];

        if((__system_property_get(name, value) > 0) &&
            (dlHandle = dlopen(value, RTLD_NOW | RTLD_LOCAL)))
        {
            pLockAcq = (perfLockAcquire)dlsym(dlHandle, "perf_lock_acq");
            pLockRel = (perfLockRelease)dlsym(dlHandle, "perf_lock_rel");
        }
        
        
        // printf("name: %s, value: %s, pLockAcq: %p, pLockRel: %p\n", name, value, pLockAcq, pLockRel);
        
        perfHandle = pLockAcq(perfHandle, ms, perfLockParamsFullUp,
                            sizeof(perfLockParamsFullUp)/sizeof(perfLockParamsFullUp[0]));
        if(perfHandle <= 0)
        {
            printf("Failed to acquire the perf lock\n");
        }
        else
        {
            perfHandle2 = pLockAcq(perfHandle, ms, perfLockParamsFullUp2,
                                sizeof(perfLockParamsFullUp2)/sizeof(perfLockParamsFullUp2[0]));
            
            if(perfHandle2 <= 0)
            {
                printf("Failed to acquire the perf lock\n");
            }
            
            // printf("perf lock acquired for %dms\n", ms);
        }
    }

    ~set_cpu()
    {
        if(perfHandle)
        {
            pLockRel(perfHandle);
            // printf("perf lock released\n");
        }
        
        if(perfHandle2)
        {
            pLockRel(perfHandle2);
            // printf("perf lock released\n");
        }
        
        if(dlHandle)
        {
            dlclose(dlHandle);
        }
    }
    
private:
    int32_t perfHandle{0};
    int32_t perfHandle2{0};
    void* dlHandle{nullptr};
    perfLockAcquire pLockAcq{nullptr};
    perfLockRelease pLockRel{nullptr};
};