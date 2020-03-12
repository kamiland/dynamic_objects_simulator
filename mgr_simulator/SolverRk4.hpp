#include <iostream>
using namespace std;

class SolverRk4
{
private:
    /* data */
public:
    SolverRk4(int stateEquationsCount);
    ~SolverRk4();

    double Solve(double step);
};
