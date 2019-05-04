#include "affinity.h"
#define debug

/* -------------------------------------
setProcessAffinity
error return -1
success return 0
takeNoEffect return 1
------------------------------------- */
int setProcessAffinity(int pid)
{
    int i, nrcpus;
    cpu_set_t mask;
    unsigned long bitmask = 0;

    CPU_ZERO(&mask);

    /* Get the CPU affinity for a pid */
    if (sched_getaffinity(pid, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }

    if (!CPU_ISSET(0, &mask))
        return 1;

    CPU_CLR(0, &mask); /* clear CPU0 from cpu set */

    /* Set the CPU affinity for a pid */
    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }

#ifdef debug
    /* get logical cpu number */
    nrcpus = sysconf(_SC_NPROCESSORS_CONF);

    for (i = 0; i < nrcpus; i++)
    {
        if (CPU_ISSET(i, &mask))
        {
            bitmask |= (unsigned long)0x01 << i;
            printf("processor #%d is set\n", i);
        }
    }
    printf("bitmask = %#lx\n", bitmask);
#endif

    return 0;
}

/* -------------------------------------
clearProcessAffinity
error return -1
success return 0
------------------------------------- */
int clearProcessAffinity(int pid)
{
    int i, nrcpus;
    cpu_set_t mask;
    unsigned long bitmask = 0;

    CPU_ZERO(&mask);

    /* Get the CPU affinity for a pid */
    if (sched_getaffinity(pid, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }

    CPU_SET(0, &mask); /* add CPU0 to cpu set */

    /* Set the CPU affinity for a pid */
    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }

#ifdef debug
    /* get logical cpu number */
    nrcpus = sysconf(_SC_NPROCESSORS_CONF);

    for (i = 0; i < nrcpus; i++)
    {
        if (CPU_ISSET(i, &mask))
        {
            bitmask |= (unsigned long)0x01 << i;
            printf("processor #%d is set\n", i);
        }
    }
    printf("bitmask = %#lx\n", bitmask);
#endif

    return 0;
}

int spareOneCore(vector<int>&operateRecord)
{
    operateRecord.clear();

    return 0;
}

int undoSpare(vector<int>&operateRecord)
{
    int len=operateRecord.size();
    for(size_t i = 0; i < len; i++)
    {
        clearProcessAffinity(operateRecord[i]);
    }
    
    return 0;
}