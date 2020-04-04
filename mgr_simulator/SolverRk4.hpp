#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

#define RK4_FIXED_K_NUMBER 4

class SolverRk4
{
private:
    int stateEquationsCount;
    double k[2][RK4_FIXED_K_NUMBER]; // hardoced for dbg

public:
    SolverRk4(int _stateEquationsCount);
    ~SolverRk4();
    
    template <class T>
    void Solve(double step, vector < double (T::*) (vector <double>) > _Odes)
    {
        int denom = 2;
        bool zCheck = false; 

        for(int j = 0; j < 4; j++)     //four steps of RK4 solver
        {
            int i = 0;
            for(auto it = begin (_Odes); it != end (_Odes); ++it)  //iterating through system of ODE's
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

                auto RK4State = new double[stateEquationsCount];

                for(int a = 0; a < stateEquationsCount; a++)
                {
                    // RK4State[a] = StateObject.State[a] + (zCheck ? (k[a][j - 1] / denom) : 0);
                    RK4State[a] = 0 + (zCheck ? (k[a][j - 1] / denom) : 0); // hardcoded for dbg
                    cout << "RK4State " << a <<": " << RK4State[a] << endl;
                }

                // k[i][j] = step * ode(new List<double>(RK4State)); //RK4 main equation, modified to system of ODE's
                k[i][j] = step * 50; // hardcoded for dbg
                
                cout << j << ": Iterating through Odes inside Solver: " << i << endl;
                cout << k[i][j] << endl;

                i++;
            }
        }

            /* updating next position of object */
        // for (int i = 0; i < StateObject.State.Count; i++)
        // {
        //     StateObject.State[i] = StateObject.State[i] + (k[i][0] + 2 * k[i][1] + 2 * k[i][2] + k[i][3]) / 6;
        // }

        cout << "[SolverRk4::Solve] pass." << endl;
        // StateObject.OperationAfterSolve();
    };
};
