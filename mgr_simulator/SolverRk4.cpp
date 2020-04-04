#include "SolverRk4.hpp"

SolverRk4::SolverRk4(int _stateEquationsCount)
{
    stateEquationsCount = _stateEquationsCount;
    auto k = new double[_stateEquationsCount][RK4_FIXED_K_NUMBER];
    memset(k, 0, sizeof(k));
}

SolverRk4::~SolverRk4()
{
}


/**
 * Prototype of generic RK4 solver in header bcs of template
 * 
*/

