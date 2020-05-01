#ifndef REFERENCE_DC_MOTOR
#define REFERENCE_DC_MOTOR

#include <iostream>
#include <vector>
using namespace std;

struct DcMotorState
{
    double rotorCurrent;
    double angularVelocity;
};


class ReferenceDcMotor
{
private:
    /* data */
    /** U - napięcie zasilania, E - siła elektromotoryczna */
    double U, E;
    /** La - indukcyjność własna wirnika, Ra - rezystancja uzwojenia wirnika, Ua - napięcie twornika    */
    double La, Ra, Ua;
    /** Lf - indukcyjność własna obwodu wzbudzenia, Rf - rezystancja obwodu wzbudzenia, If - prąd obwodu wzbudzenia     */
    double Lf, Rf, If, Ufn;
    /** Laf - indukcyjność wzajemna   */
    double Laf;
    /**  T - moment napędowy, B - współczynnik tłumienia, p - pary biegunów, Tl - moment obciążenia, J - moment bezwłądności */
    double T, B, p, Tl, J;
    /** Stała obwodu wzbudzenia (stałe magnesowanie) ifn = Ufn / Rf */
    double ifn;
    /** prędkość kątowa, prąd obwodu wirnika */
    double angularVelocity, rotorCurrent;

    double Gaf;

public:
    
    double X[2];

    ReferenceDcMotor(double angularVelocity, double rotorCurrent);
    ~ReferenceDcMotor();

    double calculateRotorCurrent(double X1, double X2, double U);
    double calculateAngularVelocity(double X1, double X2, double Tl);
    double * CalculateNextStep(double U, double h);
    vector<DcMotorState> Simulate(long numberOfProbes, double timeStep);
};

#endif