#include <iostream>
#include <vector>
#include "IStateObject.hpp"
using namespace std;


class ISolver
{
    private:

    public:
    IStateObject StateObject;
    virtual void Solve(double step);
};