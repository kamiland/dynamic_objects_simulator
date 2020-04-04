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
    std::cout << dcHistory[0].rotorCurrent << ", " << dcHistory[0].angularVelocity << '\n';

    vector <double> state;
    state.push_back(0.0);
    state.push_back(0.0);

    GenDcMotor dcmotor(0,0);
    DC_ODE temp = dcmotor.Odes;
    cout << (dcmotor.*temp[0])(state) << endl;
    cout << (dcmotor.*temp[1])(state) << endl;
    
    dcmotor.ComputeNextState(0.01);
    dcmotor.OperationAfterSolve();

    cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    // system("PAUSE");
    return 0;
}
