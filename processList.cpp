//linux head
#include <getopt.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>

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

#define unitTest

struct Process
{
    pid_t pid;
    double cpuUsage;
    double memoryUsage;
    string comm,
        task_state;
    int ppid,
        pgid,
        sid,
        tty_nr,
        tty_pgrp,
        task_flags,
        min_flt,
        cmin_flt,
        maj_flt,
        cmaj_flt,
        utime,
        stime,
        cutime,
        cstime,
        priority,
        nice,
        num_threads,
        it_real_value,
        start_time,
        vsize,
        rss;
};

int getCpuUsageInfo(int &cpuTotalTime, int &cpuIdleTime)
{
    FILE *stream;
    char buf[1000];

    if ((stream = fopen("/proc/stat", "r")) == NULL)
    {
        fprintf(stderr, "Can not open file /proc/stat.\n");
        return -1;
    }
    fclose(stream);

    fgets(buf, 1000, stream);
    int user, nice, system, idle, iowait, irq, softirq, stealstolen, guest, extTime;
    sscanf(buf + 5, "%d%d%d%d%d%d%d%d%d%d",
           &user, 
           &nice, 
           &system, 
           &idle, 
           &iowait, 
           &irq, 
           &softirq, 
           &stealstolen, 
           &guest, 
           &extTime);
    cpuTotalTime = user + nice + system + idle + iowait + irq + softirq + stealstolen + guest + extTime;
    cpuIdleTime = idle;

    return 0;
}

int getProcInfo(Process &proc, int &cpuTime)
{
    FILE *stream;
    char buf[1000];
    string fileName;
    fileName += "/proc/";
    fileName += proc.pid;
    fileName += "/stat";

    if ((stream = fopen(fileName.c_str(), "r")) == NULL)
    {
        fprintf(stderr, "Can not open file /proc/stat.\n");
        return -1;
    }
    fgets(buf, 1000, stream);
    fclose(stream);
    char comm[1000],task_state[100];
    sscanf(buf, "%*d %s %s %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
           comm,
           task_state,
           &proc.ppid,
           &proc.pgid,
           &proc.sid,
           &proc.tty_nr,
           &proc.tty_pgrp,
           &proc.task_flags,
           &proc.min_flt,
           &proc.cmin_flt,
           &proc.maj_flt,
           &proc.cmaj_flt,
           &proc.utime,
           &proc.stime,
           &proc.cutime,
           &proc.cstime,
           &proc.priority,
           &proc.nice,
           &proc.num_threads,
           &proc.it_real_value,
           &proc.start_time,
           &proc.vsize,
           &proc.rss);
    proc.comm=comm;
    proc.task_state=task_state;
    cpuTime = proc.utime + proc.stime + proc.cutime + proc.cstime;

    return 0;
}

int getCpuUsage(double &cpuUsage)
{
    int cpuTotalTime1,cpuTotalTime2,cpuIdleTime1,cpuIdleTime2;
    if(getCpuUsageInfo(cpuTotalTime1, cpuIdleTime1))return -1;
    //sleep
    if(getCpuUsageInfo(cpuTotalTime2, cpuIdleTime2))return -1;
    cpuUsage = (double)(cpuTotalTime2 - cpuTotalTime1 - cpuIdleTime2 + cpuIdleTime1) /
               (double)(cpuTotalTime2 - cpuTotalTime1);
    return 0;
}

int getProcCpuUsage(Process &proc,double &procCpuUsage)
{
    int procCpuTime1,procCpuTime2;
    int cpuTotalTime1,cpuTotalTime2,cpuIdleTime1,cpuIdleTime2;
    if(getProcInfo(proc,procCpuTime1))return -1;
    if(getCpuUsageInfo(cpuTotalTime1, cpuIdleTime1))return -1;
    usleep(1000);
    if(getProcInfo(proc,procCpuTime2))return -1;
    if(getCpuUsageInfo(cpuTotalTime2, cpuIdleTime2))return -1;
    procCpuUsage=(double)(procCpuTime2 - procCpuTime1) /
               (double)(cpuTotalTime2 - cpuTotalTime1);
}

int getProcessList(vector<Process> &processList)
{
    processList.clear();
    struct stat statInfo;
    DIR *dir;
    struct dirent *ent;

    dir = opendir("/proc");

    while ((ent = readdir(dir)) != (struct dirent *)NULL)
    {
        if (*ent->d_name > '0' && *ent->d_name <= '9')
        {
            pid_t pid;
            sscanf(ent->d_name, "%d", &pid);
            Process proc;
            proc.pid=pid;
            processList.push_back(proc);
        }
    }
    closedir(dir);
    for(size_t i = 0; i < processList.size(); i++)
    {
        double usage;
        if(!getProcCpuUsage(processList[i],usage))
        processList[i].cpuUsage=usage;
    }
    

    return 0;
}



#ifdef unitTest 
int main()
{
    vector<Process> processList;
    double usage;
    while(1)
    {
        getCpuUsage(usage);
        printf("%lf",usage);
    }
    return 0;
}

#endif