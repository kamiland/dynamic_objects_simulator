#include "DcMotor.hpp"

DcMotor::DcMotor()
{
    ext.U = 0;
    ext.Tl = 0;

    InitParameters();
    SetupODEs();
}

DcMotor::~DcMotor()
{
}

void DcMotor::InitParameters(double Ra, double La, double Rf, double Lf, double J, double B, int p, double Laf, double Ufn)
{
    this->par.Ra = Ra;   
    this->par.La = La;
    this->par.Rf = Rf;    
    this->par.Lf = Lf;
    this->par.J = J; 
    this->par.B = B;
    this->par.p = p;
    this->par.Laf = Laf;
    this->par.Ufn = Ufn;

    this->par.ifn = par.Ufn / par.Rf;
    this->par.Gaf = par.p * par.Laf * par.ifn;
    this->par.E = par.Gaf * st.AngularVelocity;
    this->par.T = par.Gaf * st.RotorCurrent;
}

double DcMotor::f1(double state[])
{
    double X1 = state[0];
    double X2 = state[1];
    return -(par.Ra / par.La) * X1 - (par.Gaf / par.La) * X2 + (1 / par.La) * ext.U;
}


double DcMotor::f2(double state[])
{
    double X1 = state[0];
    double X2 = state[1];
    return (par.Gaf / par.J) * X1 - (par.B / par.J) * X2 + (1 / par.J) * ext.Tl;
}

/** Setup Ordinary Differential Equations (ODEs) */
void DcMotor::SetupODEs()
{
    OdeList[0] = &DcMotor::f1;
    OdeList[1] = &DcMotor::f2;
}

double * DcMotor::ComputeNextState(double step, DcMotor *Object)
{
    SolverRk4 Solver(DC_MOTOR_STATE_COUNT);
    return Solver.Solve<OdeMethod, DcMotor>(step, st.State, OdeList, Object);
}

void DcMotor::OperationAfterSolve()
{

}

