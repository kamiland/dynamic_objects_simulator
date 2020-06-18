#include "GlobalContext.hpp"

GlobalContext::GlobalContext(/* args */)
{
    simulationTimeSec = 10;
    probesCountPerSec = 100;
    probesCountTotal = simulationTimeSec * probesCountPerSec;
}

GlobalContext::~GlobalContext()
{
    cout << "Destroying GlobalContext." << endl; 
}

void GlobalContext::SetSimulationTimeSec(float seconds)
{
    simulationTimeSec = seconds;
}

void GlobalContext::SetProbesCountPerSec(int probes)
{
    probesCountPerSec = probes;
}

long GlobalContext::GetProbesCountTotal()
{   
    probesCountTotal = simulationTimeSec * probesCountPerSec;
    return probesCountTotal;
}

double GlobalContext::GetStep()
{
    double Step = 1.0 / probesCountPerSec; 
    return Step; 
}