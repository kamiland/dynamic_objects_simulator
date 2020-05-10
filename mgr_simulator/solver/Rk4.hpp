#ifndef RK4
#define RK4

#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int RK4_K = 4; // four steps of RK4 solver

class SolverRk4
{
private:
    int StateEqCount;
public:
    SolverRk4(int _StateEqCount);
    ~SolverRk4();

    template <typename S, class T>
    double * Solve(double Step, double State[], S OdeList[], T *Model)
    {
        auto k = new double[StateEqCount][RK4_K];
        memset(k, 0, sizeof(k));
        int denom = 2;
        bool zCheck = false; 
        S Ode; 
        // T Model;

        for(int j = 0; j < RK4_K; j++)     // four steps of RK4 solver
        {
            for(int i = 0; i < StateEqCount; i++)  // iterating through system of ODE's
            {
                if (j > 0) { zCheck = true; } // second step of RK4
                if (j > 2) { denom = 1; } // fourth step of RK4

                auto Rk4State = new double[StateEqCount];

                for(int a = 0; a < StateEqCount; a++)
                {
                    Rk4State[a] = State[a] + (zCheck ? (k[a][j - 1] / denom) : 0);
                }
                Ode = OdeList[i];
                k[i][j] = Step * (Model->*Ode)(State); // RK4 main equation, modified to system of ODE's
            }
        }

        /* updating next position of object */
        for (int i = 0; i < StateEqCount; i++)
        {
            State[i] = State[i] + (k[i][0] + 2 * k[i][1] + 2 * k[i][2] + k[i][3]) / 6;
        }

        Model->OperationAfterSolve();
        return State;
    };
};

#endif