#include "affinity.h"
#include "processList.h"
//linux head
#include <getopt.h>
#include <sched.h>
#include <unistd.h>

//c head
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//cpp head
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// #define debug
// #define unitTest

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
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }

#ifdef debug
    /* get logical cpu number */
    nrcpus = sysconf(_SC_NPROCESSORS_CONF);
    cout << pid << endl;
    /* Get the CPU affinity for a pid */
    if (sched_getaffinity(pid, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }
    for (i = 0; i < nrcpus; i++)
    {
        if (CPU_ISSET(i, &mask))
        {
            bitmask |= (unsigned long)0x01 << i;
            printf("processor #%d is set\n", i);
        }
    }
    printf("bitmask = %#lx\n", bitmask);
    cout << "<<<<<<<<<<<<<<\n";
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
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }

#ifdef debug
    /* get logical cpu number */
    nrcpus = sysconf(_SC_NPROCESSORS_CONF);
    cout << pid << endl;
    /* Get the CPU affinity for a pid */
    if (sched_getaffinity(pid, sizeof(cpu_set_t), &mask) == -1)
    {
        return -1;
    }
    for (i = 0; i < nrcpus; i++)
    {
        if (CPU_ISSET(i, &mask))
        {
            bitmask |= (unsigned long)0x01 << i;
            printf("processor #%d is set\n", i);
        }
    }
    printf("bitmask = %#lx\n", bitmask);
    cout << "<<<<<<<<<<<<<<\n";
#endif

    return 0;
}

int spareOneCore(vector<int> &operateRecord)
{
    operateRecord.clear();
    vector<Process> processList;
    getProcessList(processList);
    for (size_t i = 0; i < processList.size(); i++)
    {
        if (processList[i].cpuUsage > 0.1)
        {
            if (setProcessAffinity(processList[i].pid))
                operateRecord.push_back(processList[i].pid);
        }
    }

    return 0;
}

int undoSpare(vector<int> &operateRecord)
{
    int len = operateRecord.size();
    for (size_t i = 0; i < len; i++)
    {
        clearProcessAffinity(operateRecord[i]);
    }

    return 0;
}

#ifdef unitTest

int unitTest1()
{
    vector<int> operateRecord;
    spareOneCore(operateRecord);
    for (size_t i = 0; i < operateRecord.size(); i++)
    {
        cout<<operateRecord[i]<<endl;
    }
    
    // undoSpare(operateRecord);
    return 0;
}

int main()
{
    unitTest1();
    return 0;
}

#endif