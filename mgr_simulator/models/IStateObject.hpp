#ifndef I_STATE_OBJECT
#define I_STATE_OBJECT

#include <iostream>
#include <vector>
using namespace std;

#define MILI(X) ((double) X / 1000)
#define MICRO(X) ((double) X / 1000000)
#define NANO(X) ((double) X / 1000000000)

class IStateObject
{
    public:
        virtual double * ComputeNextState(double step) = 0;
        virtual void OperationAfterSolve() = 0;
    
    private:
        vector <double> State;
};

#endif