#ifndef EVOLUTION
#define EVOLUTION

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "NeuralNetwork.hpp"
using namespace std;

class EvolutionaryAlgorithm
{
private:
    
public:

    unsigned int PopulationCount;
    vector <NeuralNetwork> ObjectGeneration;
    vector <NeuralNetwork> ObjectNextGeneration;
    NeuralNetwork ObjectBest;
    NeuralNetwork BestInCurrentGeneration;
    double MeanFitnessInCurrentGeneration;
    unsigned int NodesCount;
    unsigned int NonZero;

    EvolutionaryAlgorithm();
    ~EvolutionaryAlgorithm();
    void PrintGenerationFitness();
    void PrintBest();
    void FindBest();
    void EvaluateMeanFitness();
    void PickAndTweak();
    void PickAndCross();
    void Mutation();
    void Tweak();
    void Crossover();
    void ReplicateChosenOne();
    void NormalizeFitness();
    void RunSimulation();
    void EvolceNextGeneration();
};

#endif