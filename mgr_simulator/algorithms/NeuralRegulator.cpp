#include "NeuralRegulator.hpp"


NeuralRegulator::NeuralRegulator(NeuralNetwork _NeuralNet, bool RandomCc) 
: NeuralNet(_NeuralNet.NodesCount, _NeuralNet.ActivationFunctions)
{
    this->NeuralNet = _NeuralNet;

    if(RandomCc)
    {
        this->ControlConstant = ((MY_RAND * 2) - 1) * DefaultCcRrange;
    }
}

NeuralRegulator::~NeuralRegulator()
{
}

NeuralRegulator NeuralRegulator::Tweak(double weight_tweak_range, double bias_tweak_range)
{
    NeuralNetwork TweakedNN = NeuralNet.Tweak(weight_tweak_range, bias_tweak_range);
    NeuralRegulator TweakedNR(TweakedNN);
    TweakedNR.ControlConstant = this->ControlConstant + MY_RAND / 3 * this->DefaultCcTweakRange;

    return TweakedNR;
}

NeuralRegulator NeuralRegulator::Crossover(NeuralRegulator ObjectToCrossWith)
{

    NeuralNetwork CrossedNN = this->NeuralNet.Crossover(ObjectToCrossWith.NeuralNet);
    NeuralRegulator CrossedNR(CrossedNN);
    double RandomValue = MY_RAND;
    CrossedNR.ControlConstant = RandomValue * ObjectToCrossWith.ControlConstant + (1 - RandomValue) * this->ControlConstant;

    return CrossedNR;
}

double NeuralRegulator::EvaluateFitness()
{
    // function self = evaluate_fitness(self)
    // mean_count = 1;
    // self.fitness = sum(temp_fitness)/mean_count;
    // self.success = sum(temp_success)/mean_count;

    return 0.0;
}

void NeuralRegulator:: ExportNeuralRegulator()
{
    // function [control_constant, weights, biases, nodes_count] = export_neural_regulator(self)
    // control_constant = self.control_constant;
    // weights = self.neural_network.weights;
    // biases = self.neural_network.biases;
    // nodes_count = self.neural_network.nodes_count;
}

    
    
