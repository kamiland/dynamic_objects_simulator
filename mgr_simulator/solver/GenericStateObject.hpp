#include <iostream>
#include <vector>
using namespace std;

class GenericStateObject
{
private:
    vector <double> State;
    // tu powinien być wskaźnik do lity funkcji obliczających stan każdej zmiennej stanu po kolei:
    // vector <ODE> ODEs { get; } 
    // ISolver Solver { get; set; } -> to jeszcze nie wiem po co

public:
    GenericStateObject(/* args */);
    ~GenericStateObject();

    void ComputeNextState(double step);
    void OperationAfterSolve();
    double* ODE(vector <double> state); // tu był delegata
};

GenericStateObject::GenericStateObject(/* args */)
{
}

GenericStateObject::~GenericStateObject()
{
}
