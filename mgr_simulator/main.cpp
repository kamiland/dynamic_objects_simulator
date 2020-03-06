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

    /**
     * Preparing simulation parameters
    */
    context.SetSimulationTimeSec(15);
    context.SetProbesCountPerSec(1000);
    
    
    cout << "Thank you for using N-Simulator. KamilAnd." << endl;
    system("PAUSE");
    return 0;
}