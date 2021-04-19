#include "Pid.hpp"
#include <algorithm>

Controller::Controller(double InitialKp, double InitialKi, double InitialKd)
{
    P = 0;
    I = 0;
    D = 0;
    Integral = 0;
    Derivative = 0;
    Error = 0;
    PreError = 0;
    ControllerOutput = 0;
    SaturationMin = 0;
    SaturationMax = 0;
    SaturationEnable = false;

    Kp = InitialKp;
    Ki = InitialKi;
    Kd = InitialKd;
}

Controller::~Controller()
{
}

void Controller::SetSaturation(double Min, double Max)
{
    SaturationEnable = true;
    SaturationMin = Min;
    SaturationMax = Max;
}

void Controller::ResetSaturaion()
{
    SaturationEnable = false;
}

double Controller::CalculateOutput(double Setpoint, double Pv, double Dt = 0.001)
{
    Error = Setpoint - Pv;

    P = Kp * Error;

    Integral += Error * Dt;
    I = Ki * Integral;

    Derivative = (Error - PreError) / Dt;
    D = Kd * Derivative;

    PreError = Error;

    ControllerOutput = P + I + D;

    if(true == SaturationEnable)
    {
        ControllerOutput = min(ControllerOutput, SaturationMax);
        ControllerOutput = max(ControllerOutput, SaturationMin);
    }

    return ControllerOutput;
}
