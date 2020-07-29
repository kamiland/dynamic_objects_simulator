#ifndef FILTERS
#define FILTERS

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;


class Filter
{
private:
    
public:
    vector <double> Probes;
    unsigned int ProbesCount;
    double LastProbeWeight;

    Filter(unsigned int _ProbesCount, double _LastProbeWeight);
    ~Filter();
    double HighPass();
    double LowPass(double Probe);
};


#endif