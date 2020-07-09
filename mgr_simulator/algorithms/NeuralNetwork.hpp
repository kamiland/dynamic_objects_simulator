#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

#define MY_RAND ((double) rand() / (RAND_MAX))

typedef vector <vector <double>> VectorDouble2D;
typedef vector <vector <vector <double>>> VectorDouble3D;

enum ActivationFunction
{
    NONE = 0,
    RELU,
    LOG
};

double logistic(double x);
double logistic_d(double x);
double none(double x);

class NeuralNetwork
{
private:

public:
    vector <unsigned int> NodesCount;
    vector <ActivationFunction> ActivationFunctions;
    unsigned int LayersCount;
    
    VectorDouble2D Biases;
    VectorDouble3D Weights;
    VectorDouble2D Deltas;

    VectorDouble2D Layers; // not activated layers: Wx + b --> self.z_layers
    VectorDouble2D LayersActiv; // activated layers: A(Wx + b) --> self.layers
    VectorDouble2D LayersActivPrim; // derivative activated layers: A'(Wx + b) --> self.dz_layers
    VectorDouble2D LayersDeltas; // --> self.deltas_layers

    double ControlConstant;
    // double Output[];
    
    NeuralNetwork(vector <unsigned int> _NodesCount, vector <ActivationFunction> _ActivationFunctions);
    ~NeuralNetwork();
    void BiasesRandomize(double biases_range);
    void WeightsRandomize(double weights_range);
    void Randomization(double weights_range, double biases_range, double _control_constant_range);
    NeuralNetwork Tweak(double weight_tweak_range, double bias_tweak_range);
    NeuralNetwork Crossover(NeuralNetwork ObjectToCrossWith);
    template <class T, size_t N>
    vector <double> Feedforward(T (&Input)[N]);
    void Backpropagation();
};

template <class T, size_t N>
vector <double> NeuralNetwork::Feedforward(T (&Input)[N])
{   
    LayersActiv[0].insert(LayersActiv[0].begin(), begin(Input), end(Input));

    // loop through layers
    for(int layer = 0; layer < (LayersCount - 1); layer++)
    {
        // loop through nodes
        for(int node_next = 0; node_next < NodesCount[layer + 1]; node_next++)
        {
            double temp = 0.0;
            // calculate value for each node
            for(int node_current = 0; node_current < NodesCount[layer]; node_current++)
            {
                temp = temp + LayersActiv[layer][node_current] * Weights[layer][node_next][node_current];
            }   

            LayersActiv[layer + 1][node_next] = temp + Biases[layer][node_next];
            Layers[layer + 1][node_next] = LayersActiv[layer + 1][node_next];
        }

        // activation
        switch(ActivationFunctions[layer])
        {
            case NONE:
                for(int node = 0; node < LayersActiv[layer + 1].size(); node++)
                {
                    LayersActiv[layer + 1][node] = LayersActiv[layer + 1][node];
                }
                break;

            case RELU:
                for(int node = 0; node < LayersActiv[layer + 1].size(); node++)
                {
                    if(LayersActiv[layer + 1][node] < 0)
                    {
                        LayersActiv[layer + 1][node] = 0;
                    }
                    if(Layers[layer + 1][node] > 0)
                    {
                        LayersActivPrim[layer + 1][node] = 1;
                    }
                    else
                    {
                        LayersActivPrim[layer + 1][node] = 0;
                    }
                }
                break;

            case LOG:
                for(int node = 0; node < LayersActiv[layer + 1].size(); node++)
                {
                    LayersActiv[layer + 1][node] = logistic(LayersActiv[layer + 1][node]);
                    LayersActivPrim[layer + 1][node] = logistic_d(Layers[layer + 1][node]);
                }
                break;

            default:
                cout << "Not supported activation function" << endl;
                break;
        }
    }

    // *Output = *LayersActiv.back();
    return LayersActiv.back();
}

#endif