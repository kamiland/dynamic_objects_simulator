#include "NeuralNetwork.hpp"

#define MY_RAND ((double) rand() / (RAND_MAX))

NeuralNetwork::NeuralNetwork()
{
    /**
     * PSEUDOCODE
    */
    NodesCount = {5, 10, 1};
    ActivationFunctions = {RELU, RELU, LOG};
    LayersCount = NodesCount.size();

    for(int ii = 1; ii < LayersCount; ii++)
    {
        Biases.push_back(np.zeros(NodesCount[ii]));
        Deltas.push_back(np.zeros(NodesCount[ii]));
        Weights.push_back(np.zeros((NodesCount[ii], NodesCount[ii - 1])));
    }

    for(int ii = 0; ii < LayersCount; ii++)
    {
        Layers.push_back(np.zeros(NodesCount[ii]));
        LayersActiv.push_back(np.zeros(NodesCount[ii]));
        LayersActivPrim.push_back(np.zeros(NodesCount[ii]));
        LayersDeltas.push_back(np.zeros(NodesCount[ii]));
    }
}

NeuralNetwork::~NeuralNetwork()
{
}

void NeuralNetwork::WeightsRandomize(double weights_range)
{
    for(int ii = 0; ii < Weights.size(); ii++)
    {
        for(int jj = 0; jj < Weights[ii].size(); jj++)
        {
            for(int kk = 0; kk < Weights[ii][0].size(); kk++)
            {
                Weights[ii][jj][kk] = ((MY_RAND * 2) - 1) * weights_range;
            }
        }
    }
}

void NeuralNetwork::BiasesRandomize(double biases_range)
{
    for(int ii = 0; ii < Biases.size(); ii++)
    {
        for(int jj = 0; jj < Biases[ii].size(); jj++)
        {
            Biases[ii][jj] = ((MY_RAND * 2) - 1) * biases_range;
        }
    }
}

void NeuralNetwork::Randomization(double weights_range, double biases_range, double _control_constant)
{
    WeightsRandomize(weights_range);
    BiasesRandomize(biases_range);
    ControlConstant = MY_RAND * _control_constant;
}

double * NeuralNetwork::Feedforward(double Input[])
{

    /**
     * PSEUDOCODE
    */
    LayersActiv[0] = {1, 2};

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

    *Output = *LayersActiv.back();
    return LayersActiv.back();
}