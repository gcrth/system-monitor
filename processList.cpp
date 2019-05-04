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

struct Process
{
    pid_t pid;
    double cpu;
    double memory;
};

int getProcessList(vector<Process> &processList)
{
    processList.clear();
    struct stat statInfo;
    DIR *dir;
    struct dirent * ent;

    dir = opendir("/proc");

    while ((ent = readdir(dir))!=(struct dirent *)NULL)
    {
        if (*ent->d_name > '0' && *ent->d_name <= '9')
        {
            pid_t pid;
            sscanf(ent->d_name,"%D",&pid);
            processList.emplace_back(pid,0,0);
        }
    }
    closedir(dir);

    return 0;
}
