#include "EvolutionaryAlgorithm.hpp"
#include "..\models\DcMotor.hpp"


EvolutionaryAlgorithm::EvolutionaryAlgorithm(unsigned int _PopulationCount, NeuralNetwork _NeuralNet)
: ReferenceNN(_NeuralNet.NodesCount, _NeuralNet.ActivationFunctions), BestInCurrentGeneration(_NeuralNet), BestObject(_NeuralNet)
{
    this->PopulationCount = _PopulationCount;
    this->NodesCount = _NeuralNet.NodesCount;

    for(unsigned int ii = 0; ii < PopulationCount; ii++)
    {
        ReferenceNN.Randomization(1.0, 1.0, 100.0);
        NeuralRegulator NR(ReferenceNN, true);
        this->ObjectGeneration.push_back(NR);
        this->ObjectNextGeneration.push_back(NR);
    }

    BestFitness = 0.0;
}

EvolutionaryAlgorithm::~EvolutionaryAlgorithm()
{

}

void EvolutionaryAlgorithm::PrintGenerationFitness()
{
    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        cout << "Fitness of " << it - ObjectGeneration.begin() << " object: " << it->Fitness << endl;
    }
}

void EvolutionaryAlgorithm::PrintBest()
{
    cout << "Fitness of best object: " <<  BestObject.Fitness << endl;
}

NeuralRegulator EvolutionaryAlgorithm::FindBest()
{
    // Find and overwrite best
    double CurrentGenerationBestFitness = 0;

    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        if(it->Fitness > CurrentGenerationBestFitness)
        {
            CurrentGenerationBestFitness = it->Fitness;
            BestInCurrentGeneration = *it;
        }
        // if(it->Fitness > BestObject.Fitness)
        // {
        //     BestObject = *it;
        // }
        
    }
    return BestInCurrentGeneration;
}

// double EvolutionaryAlgorithm::EvaluateAverageFitness()
// {
//     // Evaluating average Fitness
//     double AverageFitness = 0.0;
//     for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
//     {
//         AverageFitness = AverageFitness + it->Fitness;
//     }
//     this->AverageFitnessInCurrentGeneration = AverageFitness / ObjectGeneration.size();
    
//     return this->AverageFitnessInCurrentGeneration;
// }

void EvolutionaryAlgorithm::PickAndTweak(unsigned int ii)
{
    NeuralRegulator Parent(ReferenceNN);
    // TODO: consider making variable instead of 0.01
    if(MY_RAND > 0.01)
    {
        bool picked = false;
        while(false == picked)
        {
            unsigned int RandomIndex = (unsigned int) floor(MY_RAND * PopulationCount);
            if(MY_RAND <= ObjectGeneration[RandomIndex].Fitness)
            {
                Parent = ObjectGeneration[RandomIndex];
                picked = true;
            }
        }
    }
    else
    {
        Parent = BestObject;
    }
    ObjectNextGeneration[ii] = Parent.Tweak();
}

void EvolutionaryAlgorithm::PickAndCross(unsigned int ii)
{
    NeuralRegulator ParentA(ReferenceNN);
    NeuralRegulator ParentB(ReferenceNN);
    unsigned int RandomIndexA = 0;
    unsigned int RandomIndexB = 0;
    bool PickedA = false;
    bool PickedB = false;

    while(false == PickedA)
    {
        RandomIndexA = (unsigned int) floor(MY_RAND * PopulationCount);
        if(MY_RAND <= ObjectGeneration[RandomIndexA].Fitness)
        {
            ParentA = ObjectGeneration[RandomIndexA];
            PickedA = true;
        }
    }
    // TODO: consider making variable instead of 0.01
    if(MY_RAND > 0.01)
    {
        PickedB = false;
        while(false == PickedB)
        {
            unsigned int RandomIndexB = (unsigned int) floor(MY_RAND * PopulationCount);
            if((RandomIndexA != RandomIndexB) && (MY_RAND <= ObjectGeneration[RandomIndexB].Fitness))
            {
                ParentB = ObjectGeneration[RandomIndexB];
                PickedB = true;
            }
        }
    }
    else
    {
        ParentB = BestObject;
    }

    ObjectNextGeneration[ii] = ParentA.Crossover(ParentB);

}

void EvolutionaryAlgorithm::Mutation(unsigned int ii)
{
    // TODO: consider making variable instead of 0.01
    if(MY_RAND > 0.01)
    {
        ReferenceNN.Randomization(0.1, 0.1, 10);
        ObjectNextGeneration[ii] = NeuralRegulator(ReferenceNN, true);
    }
    else
    {
        ObjectNextGeneration[ii] = BestObject;
    }
}

void EvolutionaryAlgorithm::ReplicateChosenOne(NeuralRegulator ChosenObject)
{
    // Replicate one chosen object
    auto itt = ObjectNextGeneration.begin();
    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it, ++itt)
    {
        *it = ChosenObject.Tweak();
        *itt = NeuralRegulator(ReferenceNN);
    }
}

void EvolutionaryAlgorithm::NormalizeFitness()
{
    NonZero = 0;
    double MaxFitness = 0.0;
    double MinFitness = 0.0;

    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        if(it->Fitness > MaxFitness)
        {
            MaxFitness = it->Fitness;
            if(MaxFitness > this->BestFitness)
            {
                this->BestFitness = MaxFitness;
                BestObject = *it;
            }
        }

    }

    MinFitness = MaxFitness;

    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        if(it->Fitness < MinFitness)
        {
            MinFitness = it->Fitness;
        }
    }

    if(MaxFitness != 0 && (MaxFitness - MinFitness) != 0)
    {
        for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
        {
            it->Fitness -= MinFitness;
            it->Fitness = (it->Fitness / (MaxFitness - MinFitness));
            if(it->Fitness > 0)
            {
                this->NonZero = NonZero + 1;
            }
        }
    }

}

void EvolutionaryAlgorithm::RunSimulation()
{
    // Run simulation
    DcMotor DcMotor;

    DcMotor.st.AngularVelocity = 0.0;
    DcMotor.st.RotorCurrent = 0.0;
    DcMotor.ext.U = 0;
    vector<double> DcHistory[2];
    double *Dc_x;
    double Input[] = {0, 0, 0};
    double ErrorIntegral = 0.0;
    double Setpoint = 150;

    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        ErrorIntegral = 0.0;
        for (int i = 0; i < 450; i++)
        {
            Input[0] = (Setpoint - DcMotor.st.AngularVelocity);
            Input[1] = DcMotor.st.AngularVelocity;
            Input[1] = DcMotor.st.RotorCurrent;
            DcMotor.ext.U = it->CalculateOutput(Input)[0];

            
            if(i == 100){ DcMotor.ext.Tl = 80;}
            if(i == 250){ DcMotor.ext.Tl = 0;}
            if(i == 350){ DcMotor.ext.Tl = -10;}

            Dc_x = DcMotor.ComputeNextState(0.01, &DcMotor);

            ErrorIntegral += pow(Setpoint - DcMotor.st.AngularVelocity, 2);

        }
        it->Fitness = 1 / (1 + ErrorIntegral);
    }

}

NeuralRegulator EvolutionaryAlgorithm::EvolveNextGeneration()
{
    double Defaults[] = {4, 0.5, 0.1, 0, 0.7};
    
    RunSimulation();
    // FindBest();
    // EvaluateAverageFitness();
    NormalizeFitness();
    // PrintGenerationFitness();
    PrintBest();
    
    unsigned int NonZeroCount = NonZero;
    double RandomValue;

    for(unsigned int ii = 0; ii < PopulationCount; ii++)
    {
        if(NonZeroCount > Defaults[0])
        {
            RandomValue = MY_RAND;
            if(RandomValue >= Defaults[1])
            {
                PickAndTweak(ii); // 50% chance of tweaking
            }
            else if(RandomValue >= Defaults[2])
            {
                PickAndCross(ii); // 40% chance of crossing
            }
            else
            {
                Mutation(ii); // 10% chance of new random object
            }
        }
        else if(NonZeroCount > Defaults[3]) // When max 4 models have non zero fitness value
        {
            RandomValue = MY_RAND;
            if(RandomValue >= Defaults[4])
            {
                PickAndTweak(ii); // 70% chance of tweaking
            }
            else
            {
                Mutation(ii); // 30% chance of new random object
            }
        }
        else 
        {
            Mutation(ii); // when all models have 0 fitness value
        }
    }

    auto itt = ObjectNextGeneration.begin();
    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it, ++itt)
    {
        *it = *itt;
    }

    return FindBest();

}
