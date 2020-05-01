/**
 * Kamil Tomasz Andrzejewski - 2020
*/

#include <iostream>
#include "GlobalContext.hpp"
#include "AllModels.hpp"
using namespace std;


int main() 
{
    /**
     * Declaration of elementary objects and variables
    */
    GlobalContext Ctx;
    Controller Pid(1, 1, 0);
    ReferenceDcMotor ReferenceMotor(0, 0);
    vector<DcMotorState> DcHistory;

    /**
     * Preparing simulation parameters
    */
    Ctx.SetSimulationTimeSec(1);
    Ctx.SetProbesCountPerSec(1000);

    DcHistory = ReferenceMotor.Simulate(Ctx.GetProbesCountTotal(), 0.001);

    DcMotor dcmotor;
    dcmotor.State[0] = 0.0;
    dcmotor.State[1] = 0.0;
    dcmotor.U = 230;
    
    vector<DcMotorState> history;
    DcMotorState state;
    double *x;

    for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
    {
        x = dcmotor.ComputeNextState(0.001);
        state.rotorCurrent = x[0];
        state.angularVelocity = x[1];
        history.push_back(state);
    }

    for(int i = 0; i < DcHistory.size(); i++)
    {
        cout << DcHistory[i].rotorCurrent << "," << DcHistory[i].angularVelocity << '\n';
    }

    for(int i = 0; i < history.size(); i++)
    {
        cout << history[i].rotorCurrent << "," << history[i].angularVelocity << '\n';
    }

    cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    return 0;
}
