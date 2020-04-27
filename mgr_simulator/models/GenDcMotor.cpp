#include "GenDcMotor.hpp"

GenDcMotor::GenDcMotor()
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

    U = 230;
    Tl = 0;

    SetupODEs();

    ifn = Ufn / Rf;
    Gaf = p * Laf * ifn;
    E = Gaf * angularVelocity;
    T = Gaf * rotorCurrent;
}

GenDcMotor::~GenDcMotor()
{
}

double GenDcMotor::f1(double state[])
{
    double X1 = state[0];
    double X2 = state[1];
    double output = -(Ra / La) * X1 - (Gaf / La) * X2 + (1 / La) * U; // for dbg
    return output;
}


double GenDcMotor::f2(double state[])
{
    double X1 = state[0];
    double X2 = state[1];
    double output = (Gaf / J) * X1 - (B / J) * X2 + (1 / J) * Tl; // for dbg
    return output;
}

/** Setup Ordinary Differential Equations (ODEs) */
void GenDcMotor::SetupODEs()
{
    OdeList[0] = &GenDcMotor::f1;
    OdeList[1] = &GenDcMotor::f2;
}

double * GenDcMotor::ComputeNextState(double step)
{
    SolverRk4 Solver(DC_MOTOR_STATE_COUNT);
    return Solver.Solve<OdeMethod, GenDcMotor>(step, State, OdeList);
}

void GenDcMotor::OperationAfterSolve()
{

}

