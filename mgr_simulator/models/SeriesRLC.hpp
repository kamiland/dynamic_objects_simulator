#ifndef SERIES_RLC
#define SERIES_RLC

// TODO:
// Add output vector interface to class

#include <iostream>
#include <vector>
#include "IStateObject.hpp"
#include "../solver/Rk4.hpp"
using namespace std;

const int SERIES_RLC_STATE_COUNT = 2;


class SeriesRLC
{   
private:
    struct _Parameters
    {
        double R;
        double L;
        double C;
    } par;

public:
    struct _ExternalForces
    {
        double U;   // input voltage
    } ext;
    
    union _State
    {
        double State[SERIES_RLC_STATE_COUNT];
        struct
        {
            double CircuitCurrent;
            double CapacitorVoltage;
        };
    } st;

    vector <double[SERIES_RLC_STATE_COUNT]> StateHistory;
    typedef double (SeriesRLC::*OdeMethod) (double[]);
    OdeMethod OdeList[SERIES_RLC_STATE_COUNT];
    OdeMethod Ode;

    SeriesRLC();
    ~SeriesRLC();
    void InitParameters(double R = 50, double L = 0.1, double C = MICRO(100));
    double f1(double state[SERIES_RLC_STATE_COUNT]);
    double f2(double state[SERIES_RLC_STATE_COUNT]);
    void SetupODEs();
    double * ComputeNextState(double step, SeriesRLC *Object);
    void OperationAfterSolve();
};


#endif