#ifndef PID
#define PID

#include <iostream>
using namespace std;

class Controller
{
private:
    double P;
    double I;
    double D;
    double Integral;
    double Derivative;
    double Error;
    double PreError;
    double ControllerOutput;
    double SaturationMin;
    double SaturationMax;
    bool SaturationEnable;

public:
    double Kp;
    double Ki;
    double Kd;

    Controller(double initialKp, double initialKi, double initialKd);
    ~Controller();

    double CalculateOutput(double setpoint, double pv, double dt);
    void SetSaturation(double min, double max);
    void ResetSaturaion();
};

#endif