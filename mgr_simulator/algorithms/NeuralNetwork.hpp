#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <iostream>
#include <vector>
#include "IStateObject.hpp"
using namespace std;

class NeuralNetwork
{
private:
    vector <unsigned int> NodesCount;
    vector <unsigned int> LayersCount; // NodesCount.len()
    vector <string> ActivationFunctions;
public:
    NeuralNetwork();
    ~NeuralNetwork();
    double Feedforward();
    void Backpropagation();
    void BiasesRandomize();
    void WeightsRandomize();
    void Randomization();



    vector <double> Biases;
    vector <double> Weights;
    vector <double> Deltas;
    vector <double> DeltasLayers;
    vector <double> LayersZ; // not activated layers: Wx + b
    vector <double> Layers; // activated layers: A(Wx + b)
    vector <double> LayersZd; // derivative activated layers: A'(Wx + b)
    

};


#endif