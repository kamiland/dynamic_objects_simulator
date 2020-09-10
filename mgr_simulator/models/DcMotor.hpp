#ifndef DC_MOTOR
#define DC_MOTOR

#include <iostream>
#include <vector>
#include "IStateObject.hpp"
#include "../solver/Rk4.hpp"
using namespace std;

const int DC_MOTOR_STATE_COUNT = 2;


class DcMotor
{   
private:
    struct _Parameters
    {
        // TODO: Translate to english
        double E;   // siła elektromotoryczna []
        double La;  // indukcyjność własna wirnika []
        double Ra;  // rezystancja uzwojenia wirnika []
        double Ua;  // napięcie twornika []
        double Lf;  // indukcyjność własna obwodu wzbudzenia []
        double Rf;  // rezystancja obwodu wzbudzenia []
        double If;  // prąd obwodu wzbudzenia []
        double Ufn; // napięcie obodu wzbudzenia []
        double Laf; // indukcyjność wzajemna []
        double T;   // moment napędowy []
        double B;   // współczynnik tłumienia []
        int p;      // liczba par biegunów []
        double J;   // moment bezwłądności []
        double ifn; // stała obwodu wzbudzenia (stałe magnesowanie) ifn = Ufn / Rf []
        double Gaf; // Gaf = p * Laf * ifn []
    } par;

public:
    struct _ExternalForces
    {
        double U;   // input voltage
        double Tl;  // Load torque
    } ext;
    
    union _State
    {
        double State[DC_MOTOR_STATE_COUNT];
        struct
        {
            double RotorCurrent;
            double AngularVelocity;
        };
    } st;

    vector <double[DC_MOTOR_STATE_COUNT]> StateHistory;
    typedef double (DcMotor::*OdeMethod) (double[]);
    OdeMethod OdeList[DC_MOTOR_STATE_COUNT];
    OdeMethod Ode;
    SolverRk4 Solver;

    DcMotor();
    ~DcMotor();
    void InitParameters(double Ra = 0.4, double La = 0.02, double Rf = 65, double Lf = 65, double J = 0.11, double B = 0.0053,
                        int p = 2, double Laf = 0.363, double Ufn = 110);
    double f1(double state[DC_MOTOR_STATE_COUNT]);
    double f2(double state[DC_MOTOR_STATE_COUNT]);
    void SetupODEs();
    double * ComputeNextState(double step, DcMotor *Object);
    void OperationAfterSolve();
};


#endif