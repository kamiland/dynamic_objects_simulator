#ifndef NEURAL_REGULATOR
#define NEURAL_REGULATOR

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "NeuralNetwork.hpp"
using namespace std;

class NeuralRegulator
{
private:
    
public:
    double Fitness = 0.0;
    double ControlConstant = 15.0;
    double DefaultCcRrange = 1;
    double DefaultCcTweakRange = 10;

    NeuralNetwork NeuralNet;
    NeuralRegulator(NeuralNetwork _NeuralNet, bool RandomCc = 0);
    ~NeuralRegulator();
    void InitNeuralNetwork();
    NeuralRegulator Tweak(double weight_tweak_range = 0.1, double bias_tweak_range = 0.1);
    NeuralRegulator Crossover(NeuralRegulator ObjectToCrossWith);
    // NeuralRegulator Mutation();
    double EvaluateFitness();
    template <class T, size_t N>
    vector <double> CalculateOutput(T (&Input)[N]);
    void ExportNeuralRegulator();
};

template <class T, size_t N>
vector <double> NeuralRegulator::CalculateOutput(T (&Input)[N])
{   
    vector <double> NnOutput = this->NeuralNet.Feedforward(Input);
    double ControlConstant = this->ControlConstant;

    transform(begin(NnOutput), end(NnOutput), begin(NnOutput), [&ControlConstant](auto& elem){return elem * ControlConstant;});

    return NnOutput;
}

#endif