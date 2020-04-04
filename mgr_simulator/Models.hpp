#include "models\Pid.hpp" 
#include "models\DcMotor.hpp"
#include "models\GenDcMotor.hpp"

typedef vector < double (GenDcMotor::*) (vector <double>) > DC_ODE;