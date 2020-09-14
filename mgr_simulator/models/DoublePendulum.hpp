#ifndef DOUBLE_PENDULUM
#define DOUBLE_PENDULUM

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "IStateObject.hpp"
#include "../solver/Rk4.hpp"
using namespace std;

const int DP_STATE_COUNT = 6;

class DoublePendulum
{
private:
    struct _Parameters
    {
        double g;       // gravity constant [m/s^2]
        double m0;      // cart mass [kg]
        double m1;      // mass of first arm [kg]
        double L1;      // lenght of first arm [m]
        double l1;      // distance to center of mass from first arm joint point [m]
        double I1;      // inertia of first arm [kg*m^2] 
        double L2;      // lenght of second arm [m]
        double I2;      // inertia of second arm [kg*m^2]
        double m2;      // mass of second arm[kg]
        double l2;      // length of second arm[m]
        double eta0;    // cart viscous friction constant [kg/s]                   // recommended value is about 0.01 
        double eta1;    // first joint viscous friction constant [(kg*m^2)/s]      // recommended value is about 0.001
        double eta2;    // second joint viscous friction constant [(kg*m^2)/s]     // recommended value is about 0.001
        double gantry;  // lenght of gantry [m]

        /* some constants to simplify the differential equations */
        double A;
        double B1;
        double B2;
        double C;
        double D1;
        double D2;
        double E;
        double F;
    } par;

public:

    struct _ExternalForces
    {
        double U;
        double Z0;
        double Z1;
        double Z2;
    } ext;

    union _State
    {
        double State[DP_STATE_COUNT];
        struct _Physical
        {
            struct _Position
            {
                double Cart;
                double InnerArm;
                double ExternalArm;
            } Position;
            struct _Velocity
            {
                double Cart;
                double InnerArm;
                double ExternalArm;
            } Velocity;
        } phy;
    } st;

    vector <double[DP_STATE_COUNT]> StateHistory;
    typedef double (DoublePendulum::*OdeMethod) (double[]);
    OdeMethod OdeList[DP_STATE_COUNT];
    OdeMethod Ode;
    SolverRk4 Solver;
    SolverType SolverType;

    DoublePendulum();
    ~DoublePendulum();
    void InitParameters(double g = 9.81, double m0 = 0.530168, double m1 = 0.18669, double L1 = 0.232039, double l1 = 0.15927,
        double I1 = 14675.631 / (1000 * 100 * 100), double L2 = 0.260, double I2 = 13518.257 / (1000 * 100 * 100), double m2 = 137.952 / 1000,
        double l2 = 12.041 / 100, double eta0 = 0.01, double eta1 = 0.001, double eta2 = 0.001, double gantry = 10.0);
    double f1(double state[DP_STATE_COUNT]);
    double f2(double state[DP_STATE_COUNT]);
    double f3(double state[DP_STATE_COUNT]);
    double f4(double state[DP_STATE_COUNT]);
    double f5(double state[DP_STATE_COUNT]);
    double f6(double state[DP_STATE_COUNT]);
    void SetupODEs();
    double * ComputeNextState(double step, DoublePendulum *Object);
    void OperationAfterSolve();

};

#endif