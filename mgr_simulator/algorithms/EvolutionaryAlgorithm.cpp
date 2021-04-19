#include "EvolutionaryAlgorithm.hpp"
#include "..\models\DcMotor.hpp"
#include "..\models\DoublePendulum.hpp"
#include "..\models\Pendulum.hpp"
#include "..\algorithms\Noise.hpp"


EvolutionaryAlgorithm::EvolutionaryAlgorithm(unsigned int _PopulationCount, NeuralNetwork _NeuralNet)
: ReferenceNN(_NeuralNet.NodesCount, _NeuralNet.ActivationFunctions), BestInCurrentGeneration(_NeuralNet), BestObject(_NeuralNet)
{
    this->PopulationCount = _PopulationCount;
    this->NodesCount = _NeuralNet.NodesCount;

    for(unsigned int ii = 0; ii < PopulationCount; ii++)
    {
        ReferenceNN.Randomization(5.0, 3.0, 10.0);
        NeuralRegulator NR(ReferenceNN, true);
        this->ObjectGeneration.push_back(NR);
        this->ObjectNextGeneration.push_back(NR);
    }

    this->ObjectGeneration[0].NeuralNet.Weights[0][0] = {-0.8451260598271784,-3.7778087905641624,-4.495302145693662,-3.015288825391952,-3.2199550234467966,0.5346956242070292};
    this->ObjectGeneration[0].NeuralNet.Weights[0][1] = {4.659810379161893,3.580504429848152,4.761243529161373,0.4110924929026056,3.190260612808638,-4.055765800625899};
    this->ObjectGeneration[0].NeuralNet.Weights[0][2] = {-0.9333249715992914,0.9677434267111067,-0.2726724132367124,0.6947550898135906,-4.656829983770397,2.273658708290991};
    this->ObjectGeneration[0].NeuralNet.Weights[0][3] = {4.266347152851728,-1.5597692241276693,-3.4228549571151565,2.5700180012642284,0.9549056704283279,-3.9174780586662377};
    this->ObjectGeneration[0].NeuralNet.Weights[0][4] = {2.728764032037844,2.117207779633802,-1.9834984789297856,-4.45183695698428,-3.2117246170814835,5.241437091175403};
    this->ObjectGeneration[0].NeuralNet.Weights[0][5] = {2.793377439719743,-1.703917326724025,-1.1824335096635576,2.694718660265931,2.0509085801204923,0.02730579944424144,};
    this->ObjectGeneration[0].NeuralNet.Weights[0][6] = {-2.349377188627033,2.5734742257701466,3.717563286397624,-3.4222323867093647,1.934700928338422,-3.161380898714448};
    this->ObjectGeneration[0].NeuralNet.Weights[0][7] = {0.397393670367422,-0.31195866181801596,2.8378419765863883,-2.3562563852563763,5.143491723887903,2.409087961362555};
    this->ObjectGeneration[0].NeuralNet.Weights[0][8] = {0.4194777953774451,-4.5869741006952385,4.159173108006858,0.4577888582092361,-3.2210012793403817,-4.115858125854505};
    this->ObjectGeneration[0].NeuralNet.Weights[0][9] = {3.8178951023731025,2.6084170716657766,-2.407915979349867,-2.4395789703897814,-2.729655965582476,-0.4745191497217785};

    this->ObjectGeneration[0].NeuralNet.Weights[1][0] = {-0.49999636315147666,1.3638868479210262,-2.832538197648306,-1.553609560700556,-1.5868928143913905,-3.029117000966784,-1.0296736259861898,2.0517479059952737,3.370599602713029,-3.5609208558236443};

    this->ObjectGeneration[0].NeuralNet.Biases[0] = {-2.208939735908202,0.09814090217057904,0.6291382740846743,-2.5858560367659407,-0.777921415097085,2.6813619956421335,1.7370555099957485,-1.6556822203191501,-1.7047956816139704,-3.1663962076384595};

    this->ObjectGeneration[0].NeuralNet.Biases[1] = {-2.723326645760562};
    
    this->ObjectGeneration[0].ControlConstant = 12.536921;

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
    ObjectNextGeneration[ii] = Parent.Tweak(0.5, 0.3);
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
        ReferenceNN.Randomization(5, 3, 15);
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

void EvolutionaryAlgorithm::RunDcSimulation()
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

void EvolutionaryAlgorithm::RunIpSimulation(nlohmann::json J)
{
    {
        Pendulum Pen;
        Pen.SolverType = J["context"]["solver_type"];
        Pen.InitParameters
        (      
            P_P["g"],
            P_P["m0"],
            P_P["m1"],
            P_P["l"],
            P_P["I"],
            P_P["eta0"],
            P_P["eta1"],
            P_P["gantry"]
        );

        Pen.st.phy.Position.Cart    =   P_S["position"]["cart"];
        Pen.st.phy.Position.Arm     =   P_S["position"]["arm"];
        Pen.st.phy.Velocity.Cart    =   P_S["velocity"]["cart"];
        Pen.st.phy.Velocity.Arm     =   P_S["velocity"]["arm"];

        Pen.ext.U   =   P_E["U"];
        Pen.ext.Z0  =   P_E["Z0"];
        Pen.ext.Z1  =   P_E["Z1"];

        vector<double> PenHistory[4];
        double *Pen_X;
        double InputPenNN[] = {0, 0, 0, 0};
        for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
        {
            for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
            {
                OutOfRange = false;
                InputPenNN[0] = Pen.st.phy.Position.Cart;
                InputPenNN[1] = Pen.st.phy.Position.Arm;
                InputPenNN[2] = Pen.st.phy.Velocity.Cart; 
                InputPenNN[3] = Pen.st.phy.Velocity.Arm;

                if (Pen.st.State[0] > 0.5) 
                {
                    OutOfRange = true;
                } else if (Pen.st.State[0] < - 0.5) 
                {
                    OutOfRange = true;
                }
                Pen.ext.U = OutOfRange ? 0 : it->CalculateOutput(InputPenNN)[0];
                
                Pen_X = Pen.ComputeNextState(Ctx.GetStep(), &Pen);

                /* new fitness function */
                if (!OutOfRange)
                {
                    auto Abs = new double[P_STATE_COUNT];
                    for (int i = 0; i < P_STATE_COUNT; i++)
                        Abs[i] = std::abs(Pen.st.State[i]);

                    double h = Ctx.GetStep();
                    for (int i = 0; i < 1; i++)
                    {
                        if (Abs[1+i] >= (3*M_PI_4))
                            it->Fitness += 0;                              //no reward in this zone
                        else if (Abs[1+i] >= M_PI_2)
                            it->Fitness += h * 1/2;                        //half point zone
                        else if (Abs[1+i] >= M_PI_4)
                            it->Fitness += h * (1 + 1/(Abs[3+i] + 1));     //one point zone + speed bonus
                        else if (Abs[1+i] >= (M_PI_4/2))
                            it->Fitness += h * (2 + 2/(Abs[3+i] + 1) + 1/(Abs[0] + 1));     //two point zone + double speed bonus + zero position bonus
                        else if (Abs[1+i] >= (M_PI_4/4))
                            it->Fitness += h * (4 + 4/(Abs[3+i] + 1) + 2/(Abs[0] + 1) + 1/(Abs[2] + 1));  //four point zone + quad speed bonus + double zero position bonus + cart velocity bonus
                        else
                            it->Fitness += h * (8 + 8/(4*Abs[3+i] + 1) + 4/(Abs[0] + 1) + 2/(4*Abs[2] + 1));  //eight point zone + octa speed bonus + quad zero position bonus + double cart velocity bonus
                    }
                }
                else
                {
                    break;
                }

                // Pen.ext.U = NR.CalculateOutput(Input)[0];
                Pen_X = Pen.ComputeNextState(Ctx.GetStep(), &Pen);
            }
        }
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
    int ii = 0;
    for(auto it = ObjectGeneration.begin(); it != ObjectGeneration.end(); ++it)
    {
        if(ii > 20) Ctx.SetSimulationTimeSec(4);
        if(ii > 40) Ctx.SetSimulationTimeSec(3);
        ii++;
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            OutOfRange = false;
            InputDipNN[0] = Dip.st.phy.Position.Cart;
            InputDipNN[1] = Dip.st.phy.Position.InnerArm;
            InputDipNN[2] = Dip.st.phy.Position.ExternalArm;
            InputDipNN[3] = Dip.st.phy.Velocity.Cart; 
            InputDipNN[4] = Dip.st.phy.Velocity.InnerArm;
            InputDipNN[5] = Dip.st.phy.Velocity.ExternalArm;

            if (std::abs(Dip.st.State[1]) >= (M_PI_4/4) || std::abs(Dip.st.State[2]) >= (M_PI_4/4))  
            {
                if (Dip.st.State[0] > 0.5) 
                {
                    // Dip.st.State[0] = 0.5;
                    // Dip.st.State[3] = 0;
                    OutOfRange = true;
                } else if (Dip.st.State[0] < - 0.5) 
                {
                    // Dip.st.State[0] = - 0.5;
                    // Dip.st.State[3] = 0;
                    OutOfRange = true;
                }
                Dip.ext.U = OutOfRange ? 0 : it->CalculateOutput(InputDipNN)[0];
            } 
            else 
            {
                // if (Dip.st.State[0] > 0.5) 
                // {
                //     // Dip.st.State[0] = 0.5;
                //     // Dip.st.State[3] = 0;
                //     OutOfRange = true;
                // } else if (Dip.st.State[0] < - 0.5) 
                // {
                //     // Dip.st.State[0] = - 0.5;
                //     // Dip.st.State[3] = 0;
                //     OutOfRange = true;
                // }
                Dip.ext.U = -Ke[0]*Dip.st.State[0] -Ke[1]*Dip.st.State[1] -Ke[2]*Dip.st.State[2] -Ke[3]*Dip.st.State[3] -Ke[4]*Dip.st.State[4] -Ke[5]*Dip.st.State[5];
            }
            
            Dip_X = Dip.ComputeNextState(Ctx.GetStep(), &Dip);

            /* new fitness function */
            if (!OutOfRange)
            {
                auto Abs = new double[DP_STATE_COUNT];
                for (int i = 0; i < DP_STATE_COUNT; i++)
                    Abs[i] = std::abs(Dip.st.State[i]);

                double h = Ctx.GetStep();
                for (int i = 0; i < 2; i++)
                {
                    if (Abs[1+i] >= (3*M_PI_4))
                        it->Fitness += 0;                              //no reward in this zone
                    else if (Abs[1+i] >= M_PI_2)
                        it->Fitness += h * 1/2;                        //half point zone
                    else if (Abs[1+i] >= M_PI_4)
                        it->Fitness += h * (1 + 1/(Abs[4+i] + 1));     //one point zone + speed bonus
                    else if (Abs[1+i] >= (M_PI_4/2))
                        it->Fitness += h * (2 + 2/(Abs[4+i] + 1) + 1/(Abs[0] + 1));     //two point zone + double speed bonus + zero position bonus
                    else if (Abs[1+i] >= (M_PI_4/4))
                        it->Fitness += h * (4 + 4/(Abs[4+i] + 1) + 2/(Abs[0] + 1) + 1/(Abs[3] + 1));  //four point zone + quad speed bonus + double zero position bonus + cart velocity bonus
                    else
                        it->Fitness += h * (8 + 8/(4*Abs[4+i] + 1) + 4/(Abs[0] + 1) + 2/(4*Abs[3] + 1));  //eight point zone + octa speed bonus + quad zero position bonus + double cart velocity bonus
                }
            }
            else
            {
                break;
            }

        }

        
    }

}



NeuralRegulator EvolutionaryAlgorithm::EvolveNextGeneration(nlohmann::json J)
{
    double Defaults[] = {4, 0.5, 0.1, 0, 0.7};
    
    RunIpSimulation(J);
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
