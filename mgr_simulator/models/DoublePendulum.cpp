#include "DoublePendulum.hpp"

DoublePendulum::DoublePendulum()
{
    InitParameters();
    SetupODEs();
}

DoublePendulum::~DoublePendulum()
{
}

void DoublePendulum::InitParameters(double g, double m0, double m1, double L1, double l1,double I1, double L2, 
                                    double I2, double m2, double l2, double eta0, double eta1, double eta2, double gantry)
{
    this->g = g;
    this->m0 = m0;
    this->m1 = m1;
    this->L1 = L1;
    this->l1 = l1;
    this->I1 = I1;
    this->L2 = L2;
    this->I2 = I2;
    this->m2 = m2;
    this->l2 = l2;
    this->eta0 = eta0;
    this->eta1 = eta1;
    this->eta2 = eta2;
    this->gantry = gantry;

    A = m0 + m1 + m2;
    B1 = m1 * l1 + m2 * L1;
    B2 = m2 * l2;
    C = m1 * l1 * l1 + m2 * L1 * L1 + I1;
    D1 = m2 * L1 * l2;
    D2 = m1 * l1 * g + m2 * L1 * g;
    E = m2 * l2 * l2 + I2;
    F = m2 * l2 * g;
}

double DoublePendulum::f1(double [])
{
    return State[3];
}

double DoublePendulum::f2(double [])
{
    return State[4];
}

double DoublePendulum::f3(double [])
{
    return State[5];
}

double DoublePendulum::f4(double [])
{
    double x2 = State[1];
    double x3 = State[2];
    double x4 = State[3];
    double x5 = State[4];
    double x6 = State[5];
    return ((U + Z0 - x4 * eta0 + B1 * sin(x2) * pow(x5, 2) + B2 * sin(x3) * pow(x6, 2)) * (C * E - pow(D1, 2) * pow((cos(x2 - x3)), 2)) + (Z1 - x5 * eta1 - (x5 - x6) * eta2 - D1 * sin(x2 - x3) * pow(x6, 2) + D2 * sin(x2)) * (B2 * cos(x3) * D1 * cos(x2 - x3) - B1 * cos(x2) * E) + (Z2 - (x6 - x5) * eta2 + D1 * sin(x2 - x3) * pow(x5, 2) + F * sin(x3)) * (B1 * cos(x2) * D1 * cos(x2 - x3) - B2 * cos(x3) * C)) / (A * C * E + 2 * B1 * cos(x2) * B2 * cos(x3) * D1 * cos(x2 - x3) - pow(B1, 2) * pow((cos(x2)), 2) * E - A * pow(D1, 2) * pow((cos(x2 - x3)), 2) - pow(B2, 2) * pow((cos(x3)), 2) * C);
}

double DoublePendulum::f5(double [])
{
    double x2 = State[1];
    double x3 = State[2];
    double x4 = State[3];
    double x5 = State[4];
    double x6 = State[5];
    return ((U + Z0 - x4 * eta0 + B1 * sin(x2) * pow(x5, 2) + B2 * sin(x3) * pow(x6, 2)) * (B2 * cos(x3) * D1 * cos(x2 - x3) - B1 * cos(x2) * E) + (Z1 - x5 * eta1 - (x5 - x6) * eta2 - D1 * sin(x2 - x3) * pow(x6, 2) + D2 * sin(x2)) * (A * E - pow(B2, 2) * pow((cos(x3)), 2)) + (Z2 - (x6 - x5) * eta2 + D1 * sin(x2 - x3) * pow(x5, 2) + F * sin(x3)) * (B1 * cos(x2) * B2 * cos(x3) - A * D1 * cos(x2 - x3))) / (A * C * E + 2 * B1 * cos(x2) * B2 * cos(x3) * D1 * cos(x2 - x3) - pow(B1, 2) * pow((cos(x2)), 2) * E - A * pow(D1, 2) * pow((cos(x2 - x3)), 2) - pow(B2, 2) * pow((cos(x3)), 2) * C);
}

double DoublePendulum::f6(double [])
{
    double x2 = State[1];
    double x3 = State[2];
    double x4 = State[3];
    double x5 = State[4];
    double x6 = State[5];
    return ((U + Z0 - x4 * eta0 + B1 * sin(x2) * pow(x5, 2) + B2 * sin(x3) * pow(x6, 2)) * (B1 * cos(x2) * D1 * cos(x2 - x3) - B2 * cos(x3) * C) + (Z1 - x5 * eta1 - (x5 - x6) * eta2 - D1 * sin(x2 - x3) * pow(x6, 2) + D2 * sin(x2)) * (B1 * cos(x2) * B2 * cos(x3) - A * D1 * cos(x2 - x3)) + (Z2 - (x6 - x5) * eta2 + D1 * sin(x2 - x3) * pow(x5, 2) + F * sin(x3)) * (A * C - pow(B1, 2) * pow((cos(x2)), 2))) / (A * C * E + 2 * B1 * cos(x2) * B2 * cos(x3) * D1 * cos(x2 - x3) - pow(B1, 2) * pow((cos(x2)), 2) * E - A * pow(D1, 2) * pow((cos(x2 - x3)), 2) - pow(B2, 2) * pow((cos(x3)), 2) * C);
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

double * DoublePendulum::ComputeNextState(double step)
{
    SolverRk4 Solver(DP_STATE_COUNT);
    return Solver.Solve<OdeMethod, DoublePendulum>(step, State, OdeList);
}

void DoublePendulum::OperationAfterSolve()
{
    for (int i = 1; i < 3; i++)
    {
        State[i] = fmod((State[i] + 3 * M_PI), (M_PI * 2)) - M_PI;
    }
}