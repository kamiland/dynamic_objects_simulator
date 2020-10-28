#include "EvolutionaryAlgorithm.hpp"
#include "..\models\DcMotor.hpp"
#include "..\models\DoublePendulum.hpp"
#include "..\algorithms\Noise.hpp"


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
    srand((unsigned) time(NULL));

    OutOfRange = false;
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
    
    Noise Noiser(0);
    Noise NoiseBig(3);  

    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        ErrorIntegral = 0.0;
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            Input[0] = (Setpoint - DcMotor.st.AngularVelocity);
            Input[1] = DcMotor.st.AngularVelocity + Noiser.Get();
            Input[2] = DcMotor.st.RotorCurrent + NoiseBig.Get();
            DcMotor.ext.U = it->CalculateOutput(Input)[0];

            
            if(i == 100){ DcMotor.ext.Tl = 80;}
            if(i == 250){ DcMotor.ext.Tl = 0;}
            if(i == 350){ DcMotor.ext.Tl = -10;}
            if(i == 550){ DcMotor.ext.Tl = 50;}
            if(i == 950){ DcMotor.ext.Tl = 0;}
            if(i == 1150){ DcMotor.ext.Tl = 63;}

            Dc_x = DcMotor.ComputeNextState(Ctx.GetStep(), &DcMotor);

            ErrorIntegral += pow(Setpoint - DcMotor.st.AngularVelocity, 2);

        }
        it->Fitness = 1 / (1 + ErrorIntegral);
    }

}


void EvolutionaryAlgorithm::RunDipSimulation(nlohmann::json J)
{
    // Run simulation
        DoublePendulum Dip;
        Dip.SolverType = J["context"]["solver_type"];
        Dip.InitParameters
        (      
            DP_P["g"],
            DP_P["m0"],
            DP_P["m1"],
            DP_P["L1"],
            DP_P["l1"],
            DP_P["I1"],
            DP_P["L2"],
            DP_P["I2"],
            DP_P["m2"],
            DP_P["l2"],
            DP_P["eta0"],
            DP_P["eta1"],
            DP_P["eta2"],
            DP_P["gantry"]
        );

        Dip.st.phy.Position.Cart        =   DP_S["position"]["cart"];
        Dip.st.phy.Position.InnerArm    =   DP_S["position"]["inner_arm"];
        Dip.st.phy.Position.ExternalArm =   DP_S["position"]["exter_arm"];
        Dip.st.phy.Velocity.Cart        =   DP_S["velocity"]["cart"];
        Dip.st.phy.Velocity.InnerArm    =   DP_S["velocity"]["inner_arm"];
        Dip.st.phy.Velocity.ExternalArm =   DP_S["velocity"]["exter_arm"];

        Dip.ext.U   =   DP_E["U"];
        Dip.ext.Z0  =   DP_E["Z0"];
        Dip.ext.Z1  =   DP_E["Z1"];
        Dip.ext.Z2  =   DP_E["Z2"];

        vector<double> DipHistory[6];
        double *Dip_X;
        double InputDipNN[] = {0, 0, 0, 0, 0, 0};

    // TODO its the same as K - to unify
    double Ke[] = {10, -259.7565, 309.8422, 8.3819, -0.7261, 27.0203};
    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            InputDipNN[0] = Dip.st.phy.Position.Cart;
            InputDipNN[1] = Dip.st.phy.Position.InnerArm;
            InputDipNN[2] = Dip.st.phy.Position.ExternalArm;
            InputDipNN[3] = Dip.st.phy.Velocity.Cart; 
            InputDipNN[4] = Dip.st.phy.Velocity.InnerArm;
            InputDipNN[5] = Dip.st.phy.Velocity.ExternalArm;

            if (std::abs(Dip.st.State[1]) >= M_PI_4/4 || std::abs(Dip.st.State[2]) >= M_PI_4/4) 
            {
                Dip.ext.U = OutOfRange ? 0 : it->CalculateOutput(InputDipNN)[0];
            } 
            else 
            {
                Dip.ext.U = -Ke[0]*Dip.st.State[0] -Ke[1]*Dip.st.State[1] -Ke[2]*Dip.st.State[2] -Ke[3]*Dip.st.State[3] -Ke[4]*Dip.st.State[4] -Ke[5]*Dip.st.State[5];
            }
            
            Dip_X = Dip.ComputeNextState(Ctx.GetStep(), &Dip);

            if (Dip.st.State[0] > 1) 
            {
                Dip.st.State[0] = 1;
                Dip.st.State[3] = 0;
                OutOfRange = true;
                break;
            } else if (Dip.st.State[0] < - 1) 
            {
                Dip.st.State[0] = - 1;
                Dip.st.State[3] = 0;
                OutOfRange = true;
                break;
            }

            /* new fitness function */
            if (!OutOfRange)
            {
                auto Abs = new double[DP_STATE_COUNT];
                for (int i = 0; i < DP_STATE_COUNT; i++)
                    Abs[i] = std::abs(Dip.st.State[i]);

                double h = Ctx.GetStep();
                for (int i = 0; i < 2; i++)
                {
                    if (Abs[1+i] >= 3*M_PI_4)
                        it->Fitness += 0;                              //no reward in this zone
                    else if (Abs[1+i] >= M_PI_2)
                        it->Fitness += h * 1/2;                        //half point zone
                    else if (Abs[1+i] >= M_PI_4)
                        it->Fitness += h * (1 + 1/(Abs[4+i] + 1));     //one point zone + speed bonus
                    else if (Abs[1+i] >= M_PI_4/2)
                        it->Fitness += h * (2 + 2/(Abs[4+i] + 1) + 1/(Abs[0] + 1));     //two point zone + double speed bonus + zero position bonus
                    else if (Abs[1+i] >= M_PI_4/4)
                        it->Fitness += h * (4 + 4/(Abs[4+i] + 1) + 2/(Abs[0] + 1) + 1/(Abs[3] + 1));  //four point zone + quad speed bonus + double zero position bonus + cart velocity bonus
                    else
                        it->Fitness += h * (8 + 8/(4*Abs[4+i] + 1) + 4/(Abs[0] + 1) + 2/(4*Abs[3] + 1));  //eight point zone + octa speed bonus + quad zero position bonus + double cart velocity bonus
                }
            }
            OutOfRange = false;

        }

        
    }

}

NeuralRegulator EvolutionaryAlgorithm::EvolveNextGeneration(nlohmann::json J)
{
    double Defaults[] = {4, 0.5, 0.1, 0, 0.7};
    
    RunDipSimulation(J);
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
