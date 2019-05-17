#include "affinity.h"
#include "processList.h"
#include "watchList.h"

//linux head
#include <getopt.h>

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

const int ERRORSTATE = -1;

static struct option const long_opts[] =
    {
        {"gui", no_argument, NULL, 1},
        {"help", optional_argument, NULL, 2},
        {NULL, 0, NULL, 0}};

void showHelp(int state)
{
    if (state == ERRORSTATE)
    {
        cout << "error for input\n";
    }
    cout << "usage as follow:\n"
         << "-h or --help for help\n"
         << "--gui for gui\n"
         << "-a for monitor all\n"
         << "-w for watch list\n";
    return;
}

int startGui()
{
    return 0;
}

int showMainMonitor()
{

    return 0;
}

int showWatchList()
{

    return 0;
}

int mainloop(int state) //msg loop
{
    bool breakFlag = false;
    switch (state) //init
    {
    case 0:
        //Main Monitor
        {
        }
        break;
    case 1:
        //Watch List
        {
        }
        break;
    }
    while (!breakFlag)
    {
        switch (state)
        {
        case 0:
            while (true) //Main Monitor
            {
                showMainMonitor();
            }
            break;
        case 1:
            while (true) //Watch List
            {
                showWatchList();
            }
            break;
        }
    }
}

int appmain(int argc, char *argv[])
{
    int c;

    while ((c = getopt_long(argc, argv, "awh", long_opts, NULL)) != -1)
    {
        switch (c)
        {
        case 'h':
        case 2: //for help
        {
            showHelp(!ERRORSTATE);
            return 0;
        }
        break;

        case 'a': //monitor all
        {
            mainloop(0);
            return 0;
        }
        break;

        case 'w': //watch some process
        {
            mainloop(1);
            return 0;
        }
        break;

        case 1: //turn to gui
        {
            startGui();
            return 0;
        }
        break;

        default: //error
        {
            showHelp(ERRORSTATE);
            exit(-1);
        }
        }
    }
    mainloop(0);
    return 0;
}

int main(int argc, char *argv[])
{
    return appmain(argc, argv);
}