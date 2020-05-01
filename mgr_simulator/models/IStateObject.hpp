#ifndef I_STATE_OBJECT
#define I_STATE_OBJECT

#include <iostream>
#include <vector>
using namespace std;

class IStateObject
{
    public:
        virtual double * ComputeNextState(double step) = 0;
        virtual void OperationAfterSolve() = 0;
    
    private:
        vector <double> State;
};

#endif