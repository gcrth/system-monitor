#ifndef processList_h
#define processList_h


#include <unistd.h>
#include <string>
#include <vector>
using namespace std;

struct Process
{
    unsigned long cpuTime,cpuTotalTime;
    pid_t pid;
    double cpuUsage;
    double memoryUsage;
    string comm,
        task_state;
    int ppid,
        pgid,
        sid,
        tty_nr,
        tty_pgrp;
    unsigned int task_flags;
    unsigned long min_flt,
        cmin_flt,
        maj_flt,
        cmaj_flt,
        utime,
        stime;
    long cutime,
        cstime,
        priority,
        nice,
        num_threads,
        it_real_value;
    unsigned long long start_time;
    unsigned long vsize;
    long rss;
    unsigned long rsslim;
};

struct MemInfo
{
    unsigned long MemTotal,
        MemFree,
        MemAvailable;//in kB
};

int getCpuUsage(double &cpuUsage);

int getMemInfo(MemInfo&memInfo);

/*>>>>>>>>>>>>>>>>>>>>
usage for getProcessList
you should give a clear vector as a param, and you can get for a vector filled of inform.
however, it may take seconds.
you should use getProcessListWithOutCpuUsage for quick response.
inform from /proc/[pid]/stat
>>>>>>>>>>>>>>>>>>>>*/
int getProcessList(vector<Process> &processList);

/*>>>>>>>>>>>>>>>>>>>>
usage for getProcInfo
for watchList.cpp only, do not use
>>>>>>>>>>>>>>>>>>>>*/
int getProcInfo(Process &proc, unsigned long &cpuTime);

/*>>>>>>>>>>>>>>>>>>>>
usage for getProcInfoIncludeCpuUsage
for watchList.cpp only, do not use
>>>>>>>>>>>>>>>>>>>>*/
int getProcInfoIncludeCpuUsage(Process &proc, double &procCpuUsage);

/*>>>>>>>>>>>>>>>>>>>>
usage for getCpuUsageInfo
get CpuUsage of the computer
>>>>>>>>>>>>>>>>>>>>*/
int getCpuUsageInfo(unsigned long &cpuTotalTime, unsigned long &cpuIdleTime);

/*>>>>>>>>>>>>>>>>>>>>
usage for getProcessListWithOutCpuUsage
you should give a clear vector as a param, 
and you can get for a vector filled of inform except CpuUsage.
you can use updateProcessList after this to get the procCpuUsage.
it gives quick response.
inform from /proc/[pid]/stat
>>>>>>>>>>>>>>>>>>>>*/
int getProcessListWithOutCpuUsage(vector<Process> &processList);

/*>>>>>>>>>>>>>>>>>>>>
usage for updateProcessList
you should give a vector from getProcessListWithOutCpuUsage or getProcessList as a param, 
and you can get for a vector filled of inform.
it gives quick response.
inform from /proc/[pid]/stat
>>>>>>>>>>>>>>>>>>>>*/
int updateProcessList(vector<Process> &processList);

#endif