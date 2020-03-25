#include <iostream>
#include <vector>
using namespace std;

class Rk4
{
private:
    /* data */
public:
    Rk4(/* args */);
    ~Rk4();
    double OperationBeforeSolve();
    double Solve();
    double OperationAfterSolve();
};

