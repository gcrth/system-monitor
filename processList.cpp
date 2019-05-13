#include "processList.h"

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

// #define unitTest

int getCpuUsageInfo(unsigned long &cpuTotalTime, unsigned long &cpuIdleTime)
{
    FILE *stream;
    char buf[1000];

    if ((stream = fopen("/proc/stat", "r")) == NULL)
    {
        fprintf(stderr, "Can not open file /proc/stat.\n");
        return -1;
    }
    fgets(buf, 1000, stream);
    fclose(stream);
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

int getProcInfo(Process &proc, unsigned long &cpuTime)
{
    FILE *stream;
    char buf[1000];
    char pidStr[100];
    sprintf(pidStr,"%d",proc.pid);
    string fileName;
    fileName += "/proc/";
    fileName += pidStr;
    fileName += "/stat";
    if ((stream = fopen(fileName.c_str(), "r")) == NULL)
    {
        fprintf(stderr, string(string("Can not open file ")+fileName+string("\n")).c_str());
        return -1;
    }
    fgets(buf, 1000, stream);
    fclose(stream);
    char comm[1000],task_state[100];

    char *front, *tail;
    front = strchr(buf, '(') + 1;
    tail = strrchr(buf, ')');
    int num = tail - front;
    if (num >= sizeof(comm))
        num = sizeof(comm) - 1;
    memcpy(comm, front, num);
    comm[num] = '\0';
    front = tail + 2;

    sscanf(front, "%s %d%d%d%d%d%u%lu%lu%lu%lu%lu%lu%ld%ld%ld%ld%ld%ld%llu%lu%ld%lu",
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
           &proc.rss,
           &proc.rsslim);
    proc.comm=comm;
    proc.task_state=task_state;
    cpuTime = proc.utime + proc.stime + proc.cutime + proc.cstime;

    return 0;
}

int getCpuUsage(double &cpuUsage)
{
    unsigned long cpuTotalTime1,cpuTotalTime2,cpuIdleTime1,cpuIdleTime2;
    if(getCpuUsageInfo(cpuTotalTime1, cpuIdleTime1))return -1;
    usleep(50000);
    if(getCpuUsageInfo(cpuTotalTime2, cpuIdleTime2))return -1;
    cpuUsage = (double)(cpuTotalTime2 - cpuTotalTime1 - cpuIdleTime2 + cpuIdleTime1) /
               (double)(cpuTotalTime2 - cpuTotalTime1);
    return 0;
}

int getProcCpuUsage(Process &proc,double &procCpuUsage)
{
    unsigned long procCpuTime1,procCpuTime2;
    unsigned long cpuTotalTime1,cpuTotalTime2,cpuIdleTime1,cpuIdleTime2;
    if(getProcInfo(proc,procCpuTime1))return -1;
    if(getCpuUsageInfo(cpuTotalTime1, cpuIdleTime1))return -1;
    usleep(50000);
    if(getProcInfo(proc,procCpuTime2))return -1;
    if(getCpuUsageInfo(cpuTotalTime2, cpuIdleTime2))return -1;
    procCpuUsage=(double)(procCpuTime2 - procCpuTime1) /
               (double)(cpuTotalTime2 - cpuTotalTime1);

    return 0;
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
        if(getProcInfo(processList[i],processList[i].cpuTime))
        {
            processList.erase(processList.begin()+i);
            continue;
        }
        processList[i].cpuUsage=usage;
    }
    

    return 0;
}

int getProcessListWithOutCpuUsageFirst(vector<Process> &processList)
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
        if(getProcInfo(processList[i],processList[i].cpuTime))
        {
            processList.erase(processList.begin()+i);
            i--;
            continue;
        }
        unsigned long time;
        getCpuUsageInfo(processList[i].cpuTotalTime,time);
        processList[i].cpuUsage=0;
    }
    

    return 0;
}

int getProcessListWithOutCpuUsageUpdate(vector<Process> &processList)
{
    vector<Process> processListNew;
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
            processListNew.push_back(proc);
        }
    }
    closedir(dir);
    for(size_t i = 0; i < processListNew.size(); i++)
    {
        double usage;
        if(getProcInfo(processListNew[i],processListNew[i].cpuTime))
        {
            processListNew.erase(processListNew.begin()+i);
            i--;
            continue;
        }
        processListNew[i].cpuUsage=0;
    }
    size_t j=0;
    for(size_t i = 0; i < processListNew.size(); i++)
    {
        while(processList[j].pid<=processListNew[i].pid)
        {
            if(processList[j].pid==processListNew[i].pid)
            {
                processListNew[i].cpuUsage=(double)(processListNew[i].cpuTime - processList[j].cpuTime) /
               (double)(processListNew[i].cpuTotalTime - processList[j].cpuTotalTime);
            }
            j++;
        }
    }
    processList=processListNew;

    return 0;
}

#ifdef unitTest 
int unitTest1()
{
    vector<Process> processList;
    double usage;
    for(size_t i = 0; i < 20; i++)
    {
        getCpuUsage(usage);
        printf("%lf\n",usage);
    }
    return 0;
}

int unitTest2()
{
    vector<Process> processList;
    double usage;
    getProcessList(processList);
    for(size_t i = 0; i < processList.size(); i++)
    {
        if(processList[i].cpuUsage==0)continue;
        cout<<"<<<<<<<<<<<<\n"
        <<processList[i].pid<<"\n"
        <<processList[i].cpuUsage<<"\n";
    }
    
    return 0;
}

int unitTest3()
{
    int sz = getpagesize();
    vector<Process> processList;
    double usage;
    getProcessList(processList);
    for(size_t i = 0; i < processList.size(); i++)
    {
        if(processList[i].rss==0)continue;
        cout<<"<<<<<<<<<<<<\n"
        <<processList[i].pid<<"\n"
        <<processList[i].rss<<"\n";
    }
    return 0;
}

int main()
{
    unitTest1();
}


#endif