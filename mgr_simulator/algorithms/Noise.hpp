#ifndef NOISE
#define NOISE

#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>
#include "MyMath.hpp"
using namespace std;

class Noise
{
private:
    
public:
    double Amplitude;
    Noise(double _Amplitude);
    ~Noise();
    void SetAmplitude(double _Amplitude);
    double Get();
};


#endif