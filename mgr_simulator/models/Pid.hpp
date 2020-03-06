#include <iostream>
using namespace std;

class Controller
{
private:
    double P;
    double I;
    double D;
    double integral;
    double derivative;
    double error;
    double pre_error;
    double controllerOutput;
    double saturationMin;
    double saturationMax;
    bool saturationEnable;

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