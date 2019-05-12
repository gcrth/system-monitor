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

int getCpuUsage(double &cpuUsage);
int getProcessList(vector<Process> &processList);

#endif