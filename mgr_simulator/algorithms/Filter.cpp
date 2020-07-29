#include "Filter.hpp"


Filter::Filter(unsigned int _ProbesCount, double _LastProbeWeight)
{
    this->ProbesCount = _ProbesCount;
    if(_LastProbeWeight > 1.0){ LastProbeWeight = 1.0; }
    else if(_LastProbeWeight < 0.0){ LastProbeWeight = 0.0; }
    this->LastProbeWeight = _LastProbeWeight;

    for(unsigned int ii = 0; ii < ProbesCount; ii++)
    {
        this->Probes.push_back(0.0);
    }
}

Filter::~Filter()
{
}

double Filter::LowPass(double Probe)
{
    // TODO: Implement weight on probes

    auto Begin = Probes.begin();
    Probes.insert(Begin, Probe);
    Probes.pop_back();
    double Y = 0.0;
    double Temp = 0.0;

    for(auto pp = Probes.begin(); pp != Probes.end(); ++pp)
    {
        Temp += *pp;
    }
    Y = Temp / ProbesCount;

    return Y;
}

