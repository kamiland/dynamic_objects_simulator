#ifndef PENDULUM
#define PENDULUM

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "IStateObject.hpp"
#include "../solver/Rk4.hpp"
using namespace std;

const int P_STATE_COUNT = 4;

class Pendulum
{
private:
    struct _Parameters
    {
        double g;       // gravity constant [m/s^2]
        double m0;      // cart mass [kg]
        double m1;      // mass of arm [kg]
        double l;      // distance to center of mass from arm joint point [m]
        double I;      // inertia of arm [kg*m^2] 
        double eta0;    // cart viscous friction constant [kg/s]             // recommended value is about 0.01 
        double eta1;    // joint viscous friction constant [(kg*m^2)/s]      // recommended value is about 0.001
        double gantry;  // lenght of gantry [m]

        double A;
        double B;
        double C;
        double D;
    } par;

public:

    struct _ExternalForces
    {
        double U;
        double Z0;
        double Z1;
    } ext;

    union _State
    {
        double State[P_STATE_COUNT];
        struct _Physical
        {
            struct _Position
            {
                double Cart;
                double Arm;
            } Position;
            struct _Velocity
            {
                double Cart;
                double Arm;
            } Velocity;
        } phy;
    } st;

    vector <double[P_STATE_COUNT]> StateHistory;
    typedef double (Pendulum::*OdeMethod) (double[]);
    OdeMethod OdeList[P_STATE_COUNT];
    OdeMethod Ode;

    Pendulum();
    ~Pendulum();
    void InitParameters(double g = 9.81, double m0 = 0.530168, double m1 = 0.18669, double l = 0.15927,
        double I = 14675.631 / (1000 * 100 * 100), double eta0 = 0.1, double eta1 = 0.01, double gantry = 10.0);
    double f1(double state[P_STATE_COUNT]);
    double f2(double state[P_STATE_COUNT]);
    double f3(double state[P_STATE_COUNT]);
    double f4(double state[P_STATE_COUNT]);
    double f5(double state[P_STATE_COUNT]);
    double f6(double state[P_STATE_COUNT]);
    void SetupODEs();
    double * ComputeNextState(double step, Pendulum *Object);
    void OperationAfterSolve();

};

#endif