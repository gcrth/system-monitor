#ifndef affinity_h
#define affinity_h

#include <vector>
using namespace std;

/*>>>>>>>>>>>>>>>>>>>>
usage for spareOneCore
you should give a clear vector as a param, and you can get for a vector filled with record
the record is for undoSpare
>>>>>>>>>>>>>>>>>>>>*/
int spareOneCore(vector<int>&operateRecord);

/*>>>>>>>>>>>>>>>>>>>>
usage for undoSpare
you should give a vector from spareOneCore as a param
>>>>>>>>>>>>>>>>>>>>*/
int undoSpare(vector<int>&operateRecord);

#endif