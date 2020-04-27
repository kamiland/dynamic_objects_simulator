/**
 * Kamil Tomasz Andrzejewski - 2020
*/

#include <iostream>
#include "GlobalContext.hpp"
#include "Models.hpp"
using namespace std;


int main() 
{
    /**
     * Declaration of elementary objects and variables
    */
    GlobalContext context;
    Controller pid(1, 1, 0);
    DcMotor motor(0, 0);
    vector<DcMotorState> dcHistory;

    /**
     * Preparing simulation parameters
    */
    context.SetSimulationTimeSec(1);
    context.SetProbesCountPerSec(1000);

    dcHistory = motor.Simulate(context.GetProbesCountTotal(), 0.001);
    // cout << "Rotor current: " << dcHistory[0].rotorCurrent << ", angular velocity: " << dcHistory[0].angularVelocity << '\n';

    GenDcMotor dcmotor;
    dcmotor.State[0] = 0.0;
    dcmotor.State[1] = 0.0;
    dcmotor.U = 230;
    
    vector<DcMotorState> history;
    DcMotorState state;
    double *x;

    for (int i = 0; i < context.GetProbesCountTotal(); i++)
    {
        x = dcmotor.ComputeNextState(0.001);
        state.rotorCurrent = x[0];
        state.angularVelocity = x[1];
        history.push_back(state);
    }

    for(int i = 0; i < dcHistory.size(); i++)
    {
        cout << dcHistory[i].rotorCurrent << "," << dcHistory[i].angularVelocity << '\n';
    }

    for(int i = 0; i < history.size(); i++)
    {
        cout << history[i].rotorCurrent << "," << history[i].angularVelocity << '\n';
    }

    cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    return 0;
}
