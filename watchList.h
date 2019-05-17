#ifndef watchList_h
#define watchList_h

#include "processList.h"

#include <vector>
using namespace std;



int saveWatchList(vector<Process>watchList);

/*>>>>>>>>>>>>>>>>>>>>
usage for openWatchList
you should give a clear vector as a param, 
and you can get for a vector filled of inform except CpuUsage.
you can use updateWatchList after this to get the procCpuUsage.
it gives quick response.
inform from /proc/[pid]/stat
>>>>>>>>>>>>>>>>>>>>*/
int openWatchList(vector<Process>&watchList);

/*>>>>>>>>>>>>>>>>>>>>
usage for updateProcessList
you should give a vector from openWatchList as a param, 
and you can get for a vector filled of inform.
the emailList is filled with the process changed.
you should send email to remind the change.
set task_state="end" for the end process.
it gives quick response.
inform from /proc/[pid]/stat
>>>>>>>>>>>>>>>>>>>>*/
int updateWatchList(vector<Process>&watchList,vector<Process>&emailList);

#endif