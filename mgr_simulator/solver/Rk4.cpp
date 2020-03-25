#include "Rk4.hpp"

Rk4::Rk4(/* args */)
{
}

Rk4::~Rk4()
{
}

double Rk4::OperationBeforeSolve()
{
    return 0;
}

double Rk4::Solve()
{
    auto k = new double[StateObject.State.Count, 4];
    int d = 2;                      //denominator in second and third step of RK4
    bool z = false;                 //adding k/d value in second to fourth step of rk4

    for (int j = 0; j < 4; j++)     //four steps of RK4 solver
    {
        int i = 0;
        for(IStateObject.ODE ode : StateObject.ODEs)  //iterating through system of ODE's
        {
            if (j > 0)      //second step of RK4
            {
                z = true;
            }
            if (j > 2)      //fourth step of RK4
            {
                d = 1;
            }

            auto RK4State = new double[StateObject.State.Count];

            for(int a = 0; a < StateObject.State.Count; a++)
            {
                RK4State[a] = StateObject.State[a] + (z ? (k[a][j - 1] / d) : 0);
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

double Rk4::OperationAfterSolve()
{
    return 0;
}


