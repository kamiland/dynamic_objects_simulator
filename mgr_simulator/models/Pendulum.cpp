#include "Pendulum.hpp"

Pendulum::Pendulum()
{
    InitParameters();
    SetupODEs();
}

Pendulum::~Pendulum()
{
}

void Pendulum::InitParameters(double g, double m0, double m1, double l, double I, double eta0, double eta1, double gantry)
{
    this->par.g = g;
    this->par.m0 = m0;
    this->par.m1 = m1;
    this->par.l = l;
    this->par.I = I;
    this->par.eta0 = eta0;
    this->par.eta1 = eta1;
    this->par.gantry = gantry;

    par.A = par.m0 + par.m1;
    par.B = par.m1 * par.l;
    par.C = par.B * par.g;
    par.D = par.B * par.l + par.I;

}

double Pendulum::f1(double state[])
{
    return state[2];
}

double Pendulum::f2(double state[])
{
    return state[3];
}

double Pendulum::f3(double state[])
{

    double x1 = state[1];
    double x2 = state[2];
    double x3 = state[3];
    return ((ext.U + ext.Z0 - par.eta0 * x2 + par.B * sin(x1) * pow(x3, 2)) * par.D - (par.B * cos(x1) * (ext.Z1 - par.eta1 * x3 - par.C * sin(x1))))
    / (par.A * par.D - pow(par.B * cos(x1), 2));
}

double Pendulum::f4(double state[])
{
    double x1 = state[1];
    double x2 = state[2];
    double x3 = state[3];
    return (par.A * (ext.Z1 - par.eta1 * x3 - par.C * sin(x1)) - ((ext.U + ext.Z0 - par.eta0 * x2 + par.B * sin(x1) * pow(x3, 2)) * par.B * cos(x1)))
    / (par.A * par.D - pow(par.B * cos(x1), 2));
}


/** Setup Ordinary Differential Equations (ODEs) */
void Pendulum::SetupODEs()
{
    OdeList[0] = &Pendulum::f1;
    OdeList[1] = &Pendulum::f2;
    OdeList[2] = &Pendulum::f3;
    OdeList[3] = &Pendulum::f4;
}

double * Pendulum::ComputeNextState(double step, Pendulum *Object)
{
    SolverRk4 Solver(P_STATE_COUNT);
    return Solver.Solve<OdeMethod, Pendulum>(step, st.State, OdeList, Object);
}

void Pendulum::OperationAfterSolve()
{
    uint16_t i = 1; 
    this->st.State[i] = fmod(st.State[i] + 3 * M_PI, M_PI * 2) - M_PI;
}