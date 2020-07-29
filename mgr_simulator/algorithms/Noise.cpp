#include "Noise.hpp"

Noise::Noise(double _Amplitude)
{
    this->Amplitude = _Amplitude;
    srand((unsigned) time(NULL));
}

Noise::~Noise()
{
}

void Noise::SetAmplitude(double _Amplitude)
{
    this->Amplitude = _Amplitude;
}

double Noise::Get()
{
    return 2 * (MY_RAND - 0.5) * Amplitude;
}