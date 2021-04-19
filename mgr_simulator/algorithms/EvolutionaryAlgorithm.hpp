#ifndef EVOLUTION
#define EVOLUTION

#include <iostream>
#include <vector>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>
#include "NeuralRegulator.hpp"
#include "..\GlobalContext.hpp"
using namespace std;

extern GlobalContext Ctx;

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
    bool OutOfRange;

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
    void RunDcSimulation();
    void RunDipSimulation(nlohmann::json J);
    void RunIpSimulation(nlohmann::json J);

    NeuralRegulator EvolveNextGeneration(nlohmann::json J);
};

#endif