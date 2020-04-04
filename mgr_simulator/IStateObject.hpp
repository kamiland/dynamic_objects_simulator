#include <iostream>
#include <vector>
using namespace std;

class IStateObject
{
    public:
        typedef vector <void (*) (vector <double>)> IODE;

        virtual void ComputeNextState(double step) = 0;
        virtual void OperationAfterSolve() = 0;
    
    private:
        vector <double> State;
        vector <IODE> Iodes;
};


