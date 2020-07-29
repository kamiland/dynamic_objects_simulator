#ifndef EVOLUTION
#define EVOLUTION

#include <iostream>
#include <vector>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>
#include "NeuralRegulator.hpp"
using namespace std;


class EvolutionaryAlgorithm
{
private:
    
public:

    unsigned int PopulationCount;
    vector <unsigned int> NodesCount;
    vector <NeuralRegulator> ObjectGeneration;
    vector <NeuralRegulator> ObjectNextGeneration;
    NeuralRegulator BestObject;
    NeuralRegulator BestInCurrentGeneration;
    double AverageFitnessInCurrentGeneration;
    unsigned int NonZero;
    NeuralNetwork ReferenceNN;
    double BestFitness;

    EvolutionaryAlgorithm(unsigned int _PopulationCount, NeuralNetwork _NeuralNet);
    ~EvolutionaryAlgorithm();
    void PrintGenerationFitness();
    void PrintBest();
    NeuralRegulator FindBest();
    double EvaluateAverageFitness();
    void PickAndTweak(unsigned int ii);
    void PickAndCross(unsigned int ii);
    void Mutation(unsigned int ii);
    void ReplicateChosenOne(NeuralRegulator ChosenObject);
    void NormalizeFitness();
    void RunSimulation();
    NeuralRegulator EvolveNextGeneration();
};

#endif