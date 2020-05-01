#ifndef ALL_MODELS
#define ALL_MODELS

#include <iostream>
using namespace std;

class GlobalContext
{
private:
    /* data */
    float simulationTimeSec = 0;
    int probesCountPerSec = 0;
    long probesCountTotal = 0;

public:
    GlobalContext(/* args */);
    ~GlobalContext();
    void SetSimulationTimeSec(float seconds);
    void SetProbesCountPerSec(int probes);
    long GetProbesCountTotal();
};

#endif