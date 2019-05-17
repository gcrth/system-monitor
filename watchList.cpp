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

#define debug
#define unitTest 

int saveWatchList(vector<Process>watchList)
{
    FILE * file;
    file=fopen("watchList.txt","w");
    for(size_t i = 0; i < watchList.size(); i++)
    {
        fprintf(file,"%d\n",watchList[i].pid);
    }
    fclose(file);
    return 0;
}

int openWatchList(vector<Process>&watchList)
{
    FILE * file;
    file=fopen("watchList.txt","r");
    pid_t pid;
    watchList.clear();
    while(fscanf(file,"%d\n",pid)!=EOF)
    {
        Process proc;
        proc.pid=pid;
        if(getProcInfo(proc, proc.cpuTime))continue;
        unsigned long time;
        getCpuUsageInfo(proc.cpuTotalTime,time);
        proc.cpuUsage=0;
        watchList.push_back(proc);
    }

    fclose(file);
    return 0;
}

int updateWatchList(vector<Process>&watchList,vector<Process>&emailList)
{
    emailList.clear();
    for(size_t i = 0; i < watchList.size(); i++)
    {
        Process proc;
        proc.pid=watchList[i].pid;
        if(getProcInfo(proc, proc.cpuTime))
        {
            watchList[i].task_state="end";
            emailList.push_back(watchList[i]);
            watchList.erase(watchList.begin()+i);
            i--;
        }
        unsigned long time;
        getCpuUsageInfo(proc.cpuTotalTime,time);
        proc.cpuUsage=(double)(proc.cpuTime - watchList[i].cpuTime) /
               (double)(proc.cpuTotalTime - watchList[i].cpuTotalTime);
        if(watchList[i].task_state!=proc.task_state)
        {
            emailList.push_back(watchList[i]);
        }
        watchList[i]=proc;
    }

    return 0;
}