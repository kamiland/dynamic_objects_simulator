#include "SeriesRLC.hpp"

SeriesRLC::SeriesRLC()
{
    ext.U = 0;

    InitParameters();
    SetupODEs();
}

SeriesRLC::~SeriesRLC()
{
}

void SeriesRLC::InitParameters(double R, double L, double C)
{
    this->par.R = R;   
    this->par.L = L;
    this->par.C = C;   
}

double SeriesRLC::f1(double state[])
{
    double X1 = state[0];
    double X2 = state[1];
    return (-par.R / par.L) * X1 - X2 / par.L + ext.U / par.L;
}


double SeriesRLC::f2(double state[])
{
    double X1 = state[0];
    double X2 = state[1];
    return X1 / par.C;
}

/** Setup Ordinary Differential Equations (ODEs) */
void SeriesRLC::SetupODEs()
{
    OdeList[0] = &SeriesRLC::f1;
    OdeList[1] = &SeriesRLC::f2;
}

double * SeriesRLC::ComputeNextState(double step, SeriesRLC *Object)
{
    SolverRk4 Solver(SERIES_RLC_STATE_COUNT);
    return Solver.Solve<OdeMethod, SeriesRLC>(step, st.State, OdeList, Object);
    // return Solver.SolveEuler<OdeMethod, SeriesRLC>(step, st.State, OdeList, Object);
}

void SeriesRLC::OperationAfterSolve()
{

}

