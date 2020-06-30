#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <iostream>
#include <vector>
#include "IStateObject.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

enum ActivationFunction
{
    NONE = 0,
    RELU,
    LOG
};

double logistic(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

double logistic_d(double x)
{
    return logistic(x) * (1.0 - logistic(x));
}

double none(double x)
{
    return x;
}
    

class NeuralNetwork
{
private:
    vector <unsigned int> NodesCount;
    unsigned int LayersCount; // NodesCount.size()
    vector <ActivationFunction> ActivationFunctions;
public:
    NeuralNetwork();
    ~NeuralNetwork();
    void BiasesRandomize(double biases_range);
    void WeightsRandomize(double weights_range);
    void Randomization(double weights_range, double biases_range, double _control_constant);
    double * Feedforward(double Input[]);
    void Backpropagation();

    vector <double[]> Biases;
    vector <double[][]> Weights;
    vector <double[]> Deltas;
    vector <double[]> Layers; // not activated layers: Wx + b --> self.z_layers
    vector <double[]> LayersActiv; // activated layers: A(Wx + b) --> self.layers
    vector <double[]> LayersActivPrim; // derivative activated layers: A'(Wx + b) --> self.dz_layers
    vector <double[]> LayersDeltas; // --> self.deltas_layers

    double ControlConstant;
    double Output[];
};

#endif