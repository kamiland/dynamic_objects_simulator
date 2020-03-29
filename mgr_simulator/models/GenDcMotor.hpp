#include <iostream>
#include <vector>
#include "../IStateObject.hpp"
using namespace std;

const int DC_MOTOR_STATE_COUNT = 2;

struct GenDcMotorState
{
    double rotorCurrent;
    double angularVelocity;
};


class GenDcMotor
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
    /** prędkość kątowa, prąd obwodu wirnika */
    double angularVelocity, rotorCurrent;

    double Gaf;

public:
    /** External forces needs to be public in order to freely apply them into the object */

    /** U - napięcie zasilania, Tl - moment obciążenia */
    double U, Tl;

    vector <double(GenDcMotor::*)(vector <double>)> ODEs;
    vector <double> State;

    GenDcMotor(double angularVelocity, double rotorCurrent);
    ~GenDcMotor();

    void ComputeNextState(double step);
    void OperationAfterSolve();

    void SetupODEs();
    double f1(vector <double> state);
    double f2(vector <double> state);
};
