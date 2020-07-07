#include "NeuralNetwork.hpp"

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

NeuralNetwork::NeuralNetwork(vector <unsigned int> _NodesCount, vector <ActivationFunction> _ActivationFunctions)
{
    NodesCount = _NodesCount;
    ActivationFunctions = _ActivationFunctions;
    LayersCount = NodesCount.size();

    vector <vector <vector <double>>> Weights(LayersCount - 1);
    vector <vector <double>> Biases(LayersCount - 1);
    vector <vector <double>> Deltas(LayersCount - 1);
    this->Weights = Weights;
    this->Biases = Biases;
    this->Deltas = Deltas;

    vector <vector <double>> Layers(LayersCount);
    vector <vector <double>> LayersActiv(LayersCount);
    vector <vector <double>> LayersActivPrim(LayersCount);
    vector <vector <double>> LayersDeltas(LayersCount);
    this->Layers = Layers;
    this->LayersActiv = LayersActiv;
    this->LayersActivPrim = LayersActivPrim;
    this->LayersDeltas = LayersDeltas;      

    for(int ii = 1; ii < LayersCount; ii++)
    {
        this->Biases[ii - 1].resize(NodesCount[ii]);
        this->Deltas[ii - 1].resize(NodesCount[ii]);
        this->Weights[ii - 1].resize(NodesCount[ii]);
        for(int jj = 0; jj < this->Weights[ii - 1].size(); jj++)
        {
            this->Weights[ii - 1][jj].resize(NodesCount[ii-1]);
        }
    }

    for(int ii = 0; ii < LayersCount; ii++)
    {
        this->Layers[ii].resize(NodesCount[ii]);
        this->LayersActiv[ii].resize(NodesCount[ii]);
        this->LayersActivPrim[ii].resize(NodesCount[ii]);
        this->LayersDeltas[ii].resize(NodesCount[ii]);
    }
}

NeuralNetwork::~NeuralNetwork()
{
}

void NeuralNetwork::WeightsRandomize(double weights_range)
{
    for (auto it = Weights.begin(); it != Weights.end(); ++it)
    {                                    
        for (auto it2 = it->begin(); it2 != it->end(); ++it2)
        {                         
            for (auto it3 = it2->begin(); it3 != it2->end(); ++it3)
            {         
                *it3 = ((MY_RAND * 2) - 1) * weights_range;
            }
        }
    }
}

void NeuralNetwork::BiasesRandomize(double biases_range)
{
    for (auto it = Biases.begin(); it != Biases.end(); ++it)
    {                                    
        for (auto it2 = it->begin(); it2 != it->end(); ++it2)
        {
            *it2 = ((MY_RAND * 2) - 1) * biases_range; 
        }
    }
}

void NeuralNetwork::Randomization(double weights_range, double biases_range, double _control_constant)
{
    WeightsRandomize(weights_range);
    BiasesRandomize(biases_range);
    ControlConstant = MY_RAND * _control_constant;
}

NeuralNetwork NeuralNetwork::Tweak(double weight_tweak_range, double bias_tweak_range)
{
    NeuralNetwork TweakedNN(this->NodesCount, this->ActivationFunctions);

    // Tweaking weights
    auto itt = this->Weights.begin();
    for(auto it = TweakedNN.Weights.begin(); it != TweakedNN.Weights.end(); ++it, ++itt)
    {
        auto itt2 = itt->begin();                     
        for(auto it2 = it->begin(); it2 != it->end(); ++it2, ++itt2)
        {
            auto itt3 = itt2->begin();                     
            for(auto it3 = it2->begin(); it3 != it2->end(); ++it3, ++itt3)
            {         
                *it3 = *itt3 + (MY_RAND / 3 * weight_tweak_range);
            }
        }
    }

    // Tweaking biases
    auto idd = this->Biases.begin();
    for (auto it = TweakedNN.Biases.begin(); it != TweakedNN.Biases.end(); ++it)
    {
        auto idd2 = idd->begin();                             
        for (auto it2 = it->begin(); it2 != it->end(); ++it2)
        {
            *it2 = *idd2 + (MY_RAND / 3 * bias_tweak_range);
        }
    }

    return TweakedNN;
}

NeuralNetwork NeuralNetwork::Crossover(NeuralNetwork ObjectToCrossWith)
{
    // Crossing weights
    NeuralNetwork CrossedNN(this->NodesCount, this->ActivationFunctions);

    auto itt = ObjectToCrossWith.Weights.begin();
    for(auto it = CrossedNN.Weights.begin(); it != CrossedNN.Weights.end(); ++it, ++itt)
    {
        auto itt2 = itt->begin();                     
        for(auto it2 = it->begin(); it2 != it->end(); ++it2, ++itt2)
        {
            auto itt3 = itt2->begin();                     
            for(auto it3 = it2->begin(); it3 != it2->end(); ++it3, ++itt3)
            {   
                double RandomValue = MY_RAND;      
                *it3 = RandomValue * *it3 + (1 - RandomValue) * *itt3;
            }
        }
    }

    // Crossing biases
    auto idd = ObjectToCrossWith.Biases.begin();
    for (auto it = CrossedNN.Biases.begin(); it != CrossedNN.Biases.end(); ++it)
    {
        auto idd2 = idd->begin();                             
        for (auto it2 = it->begin(); it2 != it->end(); ++it2)
        {
            double RandomValue = MY_RAND;
            *it2 = RandomValue * *it2 + (1 - RandomValue) * *idd2;
        }
    }

    return CrossedNN;
}
