#ifndef GLOBAL_CONTEXT
#define GLOBAL_CONTEXT

#include "json/json.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#define RLC_P J["series_rlc"]["parameters"]
#define RLC_S J["series_rlc"]["init_state"]
#define RLC_E J["series_rlc"]["external_forces"]

#define DC_P J["dc"]["parameters"]
#define DC_S J["dc"]["init_state"]
#define DC_E J["dc"]["external_forces"]

#define P_P J["pendulum"]["parameters"]
#define P_S J["pendulum"]["init_state"]
#define P_E J["pendulum"]["external_forces"]

#define DP_P J["double_pendulum"]["parameters"]
#define DP_S J["double_pendulum"]["init_state"]
#define DP_E J["double_pendulum"]["external_forces"]

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
    double GetStep();
};

#endif