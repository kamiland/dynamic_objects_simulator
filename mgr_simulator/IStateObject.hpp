#include <iostream>
#include <vector>
// #include "ISolver.hpp"
using namespace std;

class IStateObject
{
    public:
        IStateObject(/* args */);
        ~IStateObject();
        typedef vector <double (*) (vector <double>)> ODE;
        // ISolver Solver; // po co to tu jest ??

        virtual void ComputeNextState(double step);
        virtual void OperationAfterSolve();
    
    private:
        vector <double> State;
        vector <ODE> ODEs;
};


