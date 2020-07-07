// #ifndef NEURAL_REGULATOR
// #define NEURAL_REGULATOR

// #include <iostream>
// #include <vector>
// #define _USE_MATH_DEFINES
// #include <math.h>
// #include "NeuralNetwork.hpp"
// using namespace std;

// class NeuralRegulator
// {
// private:
    
// public:
//     double Fitness = 0;
//     double ControlConstant = 0;
//     double DefaultCcRrange = 10;
//     double DefaultCcTweakRange = 0.1;
//     NeuralNetwork NeuralNet;
//     NeuralRegulator(NeuralNetwork _NeuralNet, bool RandomCc = false);
//     ~NeuralRegulator();
//     NeuralRegulator Tweak(double weight_tweak_range, double bias_tweak_range);
//     NeuralRegulator Crossover(NeuralNetwork ObjectToCrossWith);
//     // NeuralRegulator Mutation();
//     double EvaluateFitness();
//     template <class T, size_t N>
//     double CalculateOutput(T (&Input)[N]);
//     void ExportNeuralRegulator();
// };

// template <class T, size_t N>
// double NeuralRegulator::CalculateOutput(T (&Input)[N])
// {   
//     vector <double> NnOutput = this->NeuralNet.Feedforward(Input);
//     return NnOutput[0] * this->ControlConstant;
// }

// #endif