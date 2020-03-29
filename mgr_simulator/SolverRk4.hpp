#include <iostream>
#include "ISolver.hpp"
using namespace std;

class SolverRk4
{
private:
    // IStateObject StateObject;

public:
    SolverRk4(int stateEquationsCount/*, IStateObject _StateObject*/);
    ~SolverRk4();

    virtual void Solve(double step);
};
