#include "Pid.hpp"

Controller::Controller(double initialKp, double initialKi, double initialKd)
{
    P = 0;
    I = 0;
    D = 0;
    integral = 0;
    derivative = 0;
    error = 0;
    pre_error = 0;
    controllerOutput = 0;
    saturationMin = 0;
    saturationMax = 0;
    saturationEnable = false;

    Kp = initialKp;
    Ki = initialKi;
    Kd = initialKd;
}

Controller::~Controller()
{
}

void Controller::SetSaturation(double min, double max)
{
    saturationEnable = true;
    saturationMin = min;
    saturationMax = max;
}

void Controller::ResetSaturaion()
{
    saturationEnable = false;
}

double Controller::CalculateOutput(double setpoint, double pv, double dt = 0.001)
{
    error = setpoint - pv;

    P = Kp * error;

    integral += error * dt;
    I = Ki * integral;

    derivative = (error - pre_error) / dt;
    D = Kd * derivative;

    pre_error = error;

    controllerOutput = P + I + D;

    if(true == saturationEnable)
    {
        controllerOutput = std::min(controllerOutput, saturationMax);
        controllerOutput = std::max(controllerOutput, saturationMin);
    }

    return controllerOutput;
}
