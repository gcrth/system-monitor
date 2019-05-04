#include <main.h>

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

bool listenerStay=true;
string order;
mutex muForOrder;
condition_variable cvForProcessOrder;
bool processed = false;
bool orderWaitToProcess=false;

int listener()
{
    while(listenerStay)
    {
        std::unique_lock<std::mutex> lck(muForOrder);
        cin>>order;  
        orderWaitToProcess=true;
        while(!processed)
        {
            cvForProcessOrder.wait(lck);
        }         
    }
}

int mainloop(int state)
{
    thread listenerThread(listener);
    bool breakFlag = false;
    while (!breakFlag)
    {
        switch (state)
        {
        case 0:
        while(true)//Main Monitor
        {

        }
        break;
        case 1:
        while(true)//Watch List
        {

        }
        break;
        }
    }
}

int showMainMonitor()
{
    return 0;
}

int showWatchList()
{
    return 0;
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
            showMainMonitor();
            return 0;
        }
        break;

        case 'w': //watch some process
        {
            showWatchList();
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
    showMainMonitor();
    return 0;
}

int main(int argc, char *argv[])
{
    return appmain(argc, argv);
}