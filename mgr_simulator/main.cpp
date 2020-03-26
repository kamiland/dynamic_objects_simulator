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
     * Declaration of elementary objescts and variables
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

    // for(DcMotorState n : dcHistory) 
    // {
    //     std::cout << n.rotorCurrent << ", " << n.angularVelocity << '\n';
    // }
    std::cout << dcHistory[0].rotorCurrent << ", " << dcHistory[0].angularVelocity << '\n';
    vector <double> state;
    state.push_back(0.0);
    state.push_back(0.0);
    GenDcMotor dcmotor(0,0);
    vector<double (GenDcMotor::*)(vector <double>)> temp = dcmotor.ODEs;
    cout << (dcmotor.*temp[0])(state) << endl;
    cout << (dcmotor.*temp[1])(state) << endl;
    
    cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    system("PAUSE");
    return 0;
}
