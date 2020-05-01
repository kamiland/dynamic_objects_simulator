#ifndef DC_MOTOR
#define DC_MOTOR

#include <iostream>
#include <vector>
#include "IStateObject.hpp"
#include "../solver/Rk4.hpp"
using namespace std;

const int DC_MOTOR_STATE_COUNT = 2;

struct GenDcMotorState
{
    double rotorCurrent;
    double angularVelocity;
};

// TODO: Translate to english

class DcMotor : IStateObject
{   
private:
    /** E - siła elektromotoryczna */
    double E;
    /** La - indukcyjność własna wirnika, Ra - rezystancja uzwojenia wirnika, Ua - napięcie twornika    */
    double La, Ra, Ua;
    /** Lf - indukcyjność własna obwodu wzbudzenia, Rf - rezystancja obwodu wzbudzenia, If - prąd obwodu wzbudzenia     */
    double Lf, Rf, If, Ufn;
    /** Laf - indukcyjność wzajemna   */
    double Laf;
    /**  T - moment napędowy, B - współczynnik tłumienia, p - pary biegunów, J - moment bezwłądności */
    double T, B, p, J;
    /** Stała obwodu wzbudzenia (stałe magnesowanie) ifn = Ufn / Rf */
    double ifn;

    double Gaf;

public:
    /** External forces needs to be public in order to freely apply them into the object */
    /** U - napięcie zasilania, Tl - moment obciążenia */
    double angularVelocity, rotorCurrent;
    double U, Tl;

    typedef double (DcMotor::*OdeMethod) (double[]);
    OdeMethod OdeList[DC_MOTOR_STATE_COUNT];
    OdeMethod Ode;
    double State[DC_MOTOR_STATE_COUNT];
    vector <double[DC_MOTOR_STATE_COUNT]> StateHistory;

    DcMotor();
    ~DcMotor();
    double * ComputeNextState(double step);
    void OperationAfterSolve();
    double f1(double state[DC_MOTOR_STATE_COUNT]);
    double f2(double state[DC_MOTOR_STATE_COUNT]);
    void SetupODEs();
};


#endif