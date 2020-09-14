#include "DoublePendulum.hpp"

DoublePendulum::DoublePendulum()
: Solver(DP_STATE_COUNT)
{
    SolverType = SOLVER_RK4;
    InitParameters();
    SetupODEs();
}

DoublePendulum::~DoublePendulum()
{
}

void DoublePendulum::InitParameters(double g, double m0, double m1, double L1, double l1,double I1, double L2, 
                                    double I2, double m2, double l2, double eta0, double eta1, double eta2, double gantry)
{
    this->par.g = g;
    this->par.m0 = m0;
    this->par.m1 = m1;
    this->par.L1 = L1;
    this->par.l1 = l1;
    this->par.I1 = I1;
    this->par.L2 = L2;
    this->par.I2 = I2;
    this->par.m2 = m2;
    this->par.l2 = l2;
    this->par.eta0 = eta0;
    this->par.eta1 = eta1;
    this->par.eta2 = eta2;
    this->par.gantry = gantry;

    par.A = par.m0 + par.m1 + par.m2;
    par.B1 = par.m1 * par.l1 + par.m2 * par.L1;
    par.B2 = par.m2 * par.l2;
    par.C = par.m1 * par.l1 * par.l1 + par.m2 * par.L1 * par.L1 + par.I1;
    par.D1 = par.m2 * par.L1 * par.l2;
    par.D2 = par.m1 * par.l1 * par.g + par.m2 * par.L1 * par.g;
    par.E = par.m2 * par.l2 * par.l2 + par.I2;
    par.F = par.m2 * par.l2 * par.g;
}

double DoublePendulum::f1(double state[])
{
    return state[3];
}

double DoublePendulum::f2(double state[])
{
    return state[4];
}

double DoublePendulum::f3(double state[])
{
    return state[5];
}

double DoublePendulum::f4(double state[])
{
    double x2 = state[1];
    double x3 = state[2];
    double x4 = state[3];
    double x5 = state[4];
    double x6 = state[5];
    return ((ext.U + ext.Z0 - x4 * par.eta0 + par.B1 * sin(x2) * pow(x5, 2) + par.B2 * sin(x3) * pow(x6, 2)) * (par.C * par.E - pow(par.D1, 2) * pow((cos(x2 - x3)), 2)) + (ext.Z1 - x5 * par.eta1 - (x5 - x6) * par.eta2 - par.D1 * sin(x2 - x3) * pow(x6, 2) + par.D2 * sin(x2)) * (par.B2 * cos(x3) * par.D1 * cos(x2 - x3) - par.B1 * cos(x2) * par.E) + (ext.Z2 - (x6 - x5) * par.eta2 + par.D1 * sin(x2 - x3) * pow(x5, 2) + par.F * sin(x3)) * (par.B1 * cos(x2) * par.D1 * cos(x2 - x3) - par.B2 * cos(x3) * par.C))
    / (par.A * par.C * par.E + 2 * par.B1 * cos(x2) * par.B2 * cos(x3) * par.D1 * cos(x2 - x3) - pow(par.B1, 2) * pow((cos(x2)), 2) * par.E - par.A * pow(par.D1, 2) * pow((cos(x2 - x3)), 2) - pow(par.B2, 2) * pow((cos(x3)), 2) * par.C);
}

double DoublePendulum::f5(double state[])
{
    double x2 = state[1];
    double x3 = state[2];
    double x4 = state[3];
    double x5 = state[4];
    double x6 = state[5];
    return ((ext.U + ext.Z0 - x4 * par.eta0 + par.B1 * sin(x2) * pow(x5, 2) + par.B2 * sin(x3) * pow(x6, 2)) * (par.B2 * cos(x3) * par.D1 * cos(x2 - x3) - par.B1 * cos(x2) * par.E) + (ext.Z1 - x5 * par.eta1 - (x5 - x6) * par.eta2 - par.D1 * sin(x2 - x3) * pow(x6, 2) + par.D2 * sin(x2)) * (par.A * par.E - pow(par.B2, 2) * pow((cos(x3)), 2)) + (ext.Z2 - (x6 - x5) * par.eta2 + par.D1 * sin(x2 - x3) * pow(x5, 2) + par.F * sin(x3)) * (par.B1 * cos(x2) * par.B2 * cos(x3) - par.A * par.D1 * cos(x2 - x3)))
    / (par.A * par.C * par.E + 2 * par.B1 * cos(x2) * par.B2 * cos(x3) * par.D1 * cos(x2 - x3) - pow(par.B1, 2) * pow((cos(x2)), 2) * par.E - par.A * pow(par.D1, 2) * pow((cos(x2 - x3)), 2) - pow(par.B2, 2) * pow((cos(x3)), 2) * par.C);
}

double DoublePendulum::f6(double state[])
{
    double x2 = state[1];
    double x3 = state[2];
    double x4 = state[3];
    double x5 = state[4];
    double x6 = state[5];
    return ((ext.U + ext.Z0 - x4 * par.eta0 + par.B1 * sin(x2) * pow(x5, 2) + par.B2 * sin(x3) * pow(x6, 2)) * (par.B1 * cos(x2) * par.D1 * cos(x2 - x3) - par.B2 * cos(x3) * par.C) + (ext.Z1 - x5 * par.eta1 - (x5 - x6) * par.eta2 - par.D1 * sin(x2 - x3) * pow(x6, 2) + par.D2 * sin(x2)) * (par.B1 * cos(x2) * par.B2 * cos(x3) - par.A * par.D1 * cos(x2 - x3)) + (ext.Z2 - (x6 - x5) * par.eta2 + par.D1 * sin(x2 - x3) * pow(x5, 2) + par.F * sin(x3)) * (par.A * par.C - pow(par.B1, 2) * pow((cos(x2)), 2)))
    / (par.A * par.C * par.E + 2 * par.B1 * cos(x2) * par.B2 * cos(x3) * par.D1 * cos(x2 - x3) - pow(par.B1, 2) * pow((cos(x2)), 2) * par.E - par.A * pow(par.D1, 2) * pow((cos(x2 - x3)), 2) - pow(par.B2, 2) * pow((cos(x3)), 2) * par.C);
}

/** Setup Ordinary Differential Equations (ODEs) */
void DoublePendulum::SetupODEs()
{
    OdeList[0] = &DoublePendulum::f1;
    OdeList[1] = &DoublePendulum::f2;
    OdeList[2] = &DoublePendulum::f3;
    OdeList[3] = &DoublePendulum::f4;
    OdeList[4] = &DoublePendulum::f5;
    OdeList[5] = &DoublePendulum::f6;
}

double * DoublePendulum::ComputeNextState(double step, DoublePendulum *Object)
{
    switch (SolverType)
    {
    case SOLVER_EULER:
        return Solver.SolveEuler<OdeMethod, DoublePendulum>(step, st.State, OdeList, Object);

    case SOLVER_RK4:
        return Solver.Solve<OdeMethod, DoublePendulum>(step, st.State, OdeList, Object);
    
    default:
        return NULL;
    }
}

void DoublePendulum::OperationAfterSolve()
{
    for (int i = 1; i < 3; i++)
    {
        // st.State[i] = fmod((st.State[i] + 3 * M_PI), (M_PI * 2)) - M_PI;
    }
}