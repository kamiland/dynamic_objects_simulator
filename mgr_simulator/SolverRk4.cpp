#include "SolverRk4.hpp"
#define RK4_FIXED_K_NUMBER 4

SolverRk4::SolverRk4(int stateEquationsCount)
{
    auto k = new double[stateEquationsCount][RK4_FIXED_K_NUMBER];
}

SolverRk4::~SolverRk4()
{
}

double SolverRk4::Solve(double step)
{
    int denom = 2;
    bool zCheck = false; 

    for(int j = 0; j < 4; j++)     //four steps of RK4 solver
    {
        int i = 0;
        for(IStateObject.ODE ode : StateObject.ODEs)  //iterating through system of ODE's
        {
            //second step of RK4
            if (j > 0)
            {
                zCheck = true;
            }      
            //fourth step of RK4
            if (j > 2)
            {
                denom = 1;
            }

            double[] RK4State = new double[stateEquationsCount];

            for(int a = 0; a < stateEquationsCount; a++)
            {
                RK4State[a] = StateObject.State[a] + (zCheck ? (k[a, j - 1] / denom) : 0);
            }

            k[i][j] = step * ode(new List<double>(RK4State)); //RK4 main equation, modified to system of ODE's
            i++;
        }
    }

        /* updating next position of object */
    for (int i = 0; i < StateObject.State.Count; i++)
    {
        StateObject.State[i] = StateObject.State[i] + (k[i][0] + 2 * k[i][1] + 2 * k[i][2] + k[i][3]) / 6;
    }

    StateObject.OperationAfterSolve();
}
