#include "DcMotor.hpp"

DcMotor::DcMotor(double rotorCurrent, double angularVelocity)
{
    /** Parametry domyślne  */
    Ra = 0.4;   
    La = 0.02;
    Rf = 65;    
    Lf = 65;
    J = 0.11; 
    B = 0.0053;
    p = 2;
    Laf = 0.363;
    Ufn = 110;

    /** Wartości początkowe dla obiektu silnika   */
    X[0] = rotorCurrent;
    X[1] = angularVelocity;

    ifn = Ufn / Rf;
    Gaf = p * Laf * ifn;
    E = Gaf * angularVelocity;
    T = Gaf * rotorCurrent;
}

DcMotor::~DcMotor()
{
}


double DcMotor::calculateRotorCurrent(double X1, double X2, double U)
{
    return -(Ra / La) * X1 - (Gaf / La) * X2 + (1 / La) * U;
}


double DcMotor::calculateAngularVelocity(double X1, double X2, double Tl)
{
    return (Gaf / J) * X1 - (B / J) * X2 + (1 / J) * Tl;
}

// implementacja algorytmu Rungego-Kutty dla obiektu silnika DC (https://pl.wikipedia.org/wiki/Algorytm_Rungego-Kutty)
double * DcMotor::CalculateNextStep(double U, double h)
{
    auto k = new double[2][4];

    /** Wyznaczanie prądu obwodu wirnika    */
    k[0][0] = h * calculateRotorCurrent(X[0], X[1], U);
    k[0][1] = h * calculateRotorCurrent(X[0] + k[0][0] / 2, X[1] + k[0][0] / 2, U);
    k[0][2] = h * calculateRotorCurrent(X[0] + k[0][1] / 2, X[1] + k[0][1] / 2, U);
    k[0][3] = h * calculateRotorCurrent(X[0] + k[0][2], X[1] + k[0][2], U);

    /** Wyznaczanie prędkości kątowej */
    k[1][0] = h * calculateAngularVelocity(X[0], X[1], 0);
    k[1][1] = h * calculateAngularVelocity(X[0] + k[1][0] / 2, X[1] + k[1][0] / 2, 0);
    k[1][2] = h * calculateAngularVelocity(X[0] + k[1][1] / 2, X[1] + k[1][1] / 2, 0);
    k[1][3] = h * calculateAngularVelocity(X[0] + k[1][2], X[1] + k[1][2], 0);

    for (int i = 0; i < 2; i++)
    {
        X[i] = X[i] + (k[i][0] + 2 * k[i][1] + 2 * k[i][2] + k[i][3]) / 6;
    }
    return X;
}

vector<DcMotorState> DcMotor::Simulate(long numberOfProbes, double timeStep)
{
    X[0] = 0; // rotor current
    X[1] = 0; // angular velocity

    vector<DcMotorState> history;
    DcMotorState state;
    double *x;

    for (int i = 0; i < numberOfProbes; i++)
    {
        x = CalculateNextStep(230, timeStep);
        state.rotorCurrent = x[0];
        state.angularVelocity = x[1];
        history.push_back(state);
    }

    return history;
}