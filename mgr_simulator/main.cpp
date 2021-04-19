/**
 * Kamil Tomasz Andrzejewski - 2020
*/

#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <windows.h>
#include <algorithm>
#include "GlobalContext.hpp"
#include "AllModels.hpp"
#include "UsefulFunctions.hpp"

using namespace std;
using json = nlohmann::json;

GlobalContext Ctx;

int main() 
{
    /**
     * Reading JSON with configuration of simulation
     * 
    */
    json J;
    string RawJsonString = ReadFromFile("./json/simulation_context.json");
    J = json::parse(RawJsonString);


    /**
     * Preparing simulation context and parameters
     * 
    */
    // GlobalContext Ctx;
    Ctx.SetSimulationTimeSec(J["context"]["time"]);
    Ctx.SetProbesCountPerSec(J["context"]["probes_per_second"]);

    auto Now = chrono::system_clock::now();
    auto DateStamp = chrono::system_clock::to_time_t(Now);
    srand((unsigned int) DateStamp);


    /**
     * Declaration of elementary objects and variables
     * 
    */
    vector <unsigned int> _NodesCount = J["neural_network"]["nodes_count"];
    vector <ActivationFunction> _ActivationFunctions = J["neural_network"]["activation_functions"];
    NeuralNetwork NN(_NodesCount, _ActivationFunctions);
    
    NN.Randomization(J["neural_network"]["weights_range"], J["neural_network"]["biases_range"], 
                     J["neural_network"]["control_constant_range"]);

    NeuralRegulator NR(NN);


    /**
     * Execution of simulation and research
     * 
    */
    auto StartEvo = std::chrono::high_resolution_clock::now(); 

    EvolutionaryAlgorithm EVA(J["evo"]["population"], NN);
    for (int i = 0; i < J["evo"]["generation"]; i++)
    {
        NR = EVA.EvolveNextGeneration(J);
    }


    // Run simulation
    auto StartSim = std::chrono::high_resolution_clock::now(); 


    ///////////////// RLC /////////////////
    if(J["onoff"]["series_rlc"])
    {
        SeriesRLC Rlc;
        Rlc.SolverType = J["context"]["solver_type"];
        Rlc.InitParameters
        (
            RLC_P["r"],
            RLC_P["l"],
            RLC_P["c"]
        );

        Rlc.st.CapacitorVoltage =   RLC_S["capacitor_voltage"];
        Rlc.st.CircuitCurrent   =   RLC_S["circuit_current"];

        vector<double> RlcHistory[2];
        double *Rlc_X;

        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {

            Rlc_X = Rlc.ComputeNextState(Ctx.GetStep(), &Rlc);

            RlcHistory[0].push_back(Rlc_X[0]);
            RlcHistory[1].push_back(Rlc_X[1]);
        }
        WriteToFile(RlcHistory, "rlc");
    }


    ///////////////// DC MOTOR /////////////////
    if(J["onoff"]["dc"])
    {
        DcMotor DcMotor;
        DcMotor.SolverType = J["context"]["solver_type"];
        DcMotor.InitParameters
        (
            DC_P["ra"],
            DC_P["la"],
            DC_P["rf"],
            DC_P["lf"],
            DC_P["j"],
            DC_P["b"],
            DC_P["p"],
            DC_P["laf"],
            DC_P["ufn"]
        );

        DcMotor.st.AngularVelocity  =   DC_S["angular_velocity"];
        DcMotor.st.RotorCurrent     =   DC_S["rotor_current"];

        DcMotor.ext.U   =   DC_E["u"];
        DcMotor.ext.Tl  =   DC_E["tl"];

        vector<double> DcHistory[5];
        double *Dc_x;
        double Input[] = {0, 0, 0};
        double ErrorIntegral = 0.0;
        double Setpoint = 150;

        Noise Noiser(0);
        Noise NoiseBig(3);
        Filter LPF(4, 0.5);
        double filtered = 0.0;
        Controller Pid(J["pid"]["kp"], J["pid"]["ki"], J["pid"]["kd"]);
        if (true == J["pid"]["saturation"]["set"])
        {
            Pid.SetSaturation(J["pid"]["saturation"]["min"], J["pid"]["saturation"]["max"]);
        }
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            Input[0] = (Setpoint - DcMotor.st.AngularVelocity);
            Input[1] = DcMotor.st.AngularVelocity + Noiser.Get();
            Input[2] = DcMotor.st.RotorCurrent + NoiseBig.Get();
            // DcMotor.ext.U = 150; //NR.CalculateOutput(Input)[0];
            DcMotor.ext.U = Pid.CalculateOutput(150, Input[1], Ctx.GetStep());
            filtered = LPF.LowPass(DcMotor.st.RotorCurrent + NoiseBig.Get());

            if(i == 80){ DcMotor.ext.Tl = 15;}
            if(i == 190){ DcMotor.ext.Tl = 0;}
            if(i == 310){ DcMotor.ext.Tl = -7;}
            if(i == 650){ DcMotor.ext.Tl = 20;}
            if(i == 850){ DcMotor.ext.Tl = -13;}
            if(i == 1200){ DcMotor.ext.Tl = 29;}
            if(i == 1600){ DcMotor.ext.Tl = 0;}

            Dc_x = DcMotor.ComputeNextState(Ctx.GetStep(), &DcMotor);

            DcHistory[0].push_back(Dc_x[0]);
            DcHistory[1].push_back(Dc_x[1]);
            DcHistory[2].push_back(DcMotor.ext.U);
            DcHistory[3].push_back(Input[2]);
            DcHistory[4].push_back(DcMotor.ext.Tl);
        }
        WriteToFile(DcHistory, "dc");
    }


    ///////////////// PENDULUM /////////////////
    if(J["onoff"]["pendulum"])
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
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            InputPenNN[0] = Pen.st.phy.Position.Cart;
            InputPenNN[1] = Pen.st.phy.Position.Arm;
            InputPenNN[2] = Pen.st.phy.Velocity.Cart; 
            InputPenNN[3] = Pen.st.phy.Velocity.Arm;

            Pen.ext.U = NR.CalculateOutput(InputPenNN)[0];
            Pen.ext.U = 0;
            Pen_X = Pen.ComputeNextState(Ctx.GetStep(), &Pen);

            PenHistory[0].push_back(Pen_X[0]);
            PenHistory[1].push_back(Pen_X[1]);
            PenHistory[2].push_back(Pen_X[2]);
            PenHistory[3].push_back(Pen_X[3]);
        }
        WriteToFile(PenHistory, "pen");
    }
    

    ///////////////// DOUBLE PENDULUM /////////////////
    #define M_PI_4		0.78539816339744830962
    #define M_PI		3.14159265358979323846
    if(J["onoff"]["double_pendulum"])
    {
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
        /* control gains */
        double K[] = {10, -259.7565, 309.8422, 8.3819, -0.7261, 27.0203};   
        int TimeZ = 0;
        bool OutOfRange = false;
        Ctx.SetSimulationTimeSec(7);

        // NR.NeuralNet.Weights[0][0] = {-0.8451260598271784,-3.7778087905641624,-4.495302145693662,-3.015288825391952,-3.2199550234467966,0.5346956242070292};
        // NR.NeuralNet.Weights[0][1] = {4.659810379161893,3.580504429848152,4.761243529161373,0.4110924929026056,3.190260612808638,-4.055765800625899};
        // NR.NeuralNet.Weights[0][2] = {-0.9333249715992914,0.9677434267111067,-0.2726724132367124,0.6947550898135906,-4.656829983770397,2.273658708290991};
        // NR.NeuralNet.Weights[0][3] = {4.266347152851728,-1.5597692241276693,-3.4228549571151565,2.5700180012642284,0.9549056704283279,-3.9174780586662377};
        // NR.NeuralNet.Weights[0][4] = {2.728764032037844,2.117207779633802,-1.9834984789297856,-4.45183695698428,-3.2117246170814835,5.241437091175403};
        // NR.NeuralNet.Weights[0][5] = {2.793377439719743,-1.703917326724025,-1.1824335096635576,2.694718660265931,2.0509085801204923,0.02730579944424144,};
        // NR.NeuralNet.Weights[0][6] = {-2.349377188627033,2.5734742257701466,3.717563286397624,-3.4222323867093647,1.934700928338422,-3.161380898714448};
        // NR.NeuralNet.Weights[0][7] = {0.397393670367422,-0.31195866181801596,2.8378419765863883,-2.3562563852563763,5.143491723887903,2.409087961362555};
        // NR.NeuralNet.Weights[0][8] = {0.4194777953774451,-4.5869741006952385,4.159173108006858,0.4577888582092361,-3.2210012793403817,-4.115858125854505};
        // NR.NeuralNet.Weights[0][9] = {3.8178951023731025,2.6084170716657766,-2.407915979349867,-2.4395789703897814,-2.729655965582476,-0.4745191497217785};

        // NR.NeuralNet.Weights[1][0] = {-0.49999636315147666,1.3638868479210262,-2.832538197648306,-1.553609560700556,-1.5868928143913905,-3.029117000966784,-1.0296736259861898,2.0517479059952737,3.370599602713029,-3.5609208558236443};

        // NR.NeuralNet.Biases[0] = {-2.208939735908202,0.09814090217057904,0.6291382740846743,-2.5858560367659407,-0.777921415097085,2.6813619956421335,1.7370555099957485,-1.6556822203191501,-1.7047956816139704,-3.1663962076384595};

        // NR.NeuralNet.Biases[1] = {-2.723326645760562};
        
        // NR.ControlConstant = 12.536921;

        // Dip.st.State[1] = M_PI;
        // Dip.st.State[2] = M_PI;
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            InputDipNN[0] = Dip.st.phy.Position.Cart;
            InputDipNN[1] = Dip.st.phy.Position.InnerArm;
            InputDipNN[2] = Dip.st.phy.Position.ExternalArm;
            InputDipNN[3] = Dip.st.phy.Velocity.Cart; 
            InputDipNN[4] = Dip.st.phy.Velocity.InnerArm;
            InputDipNN[5] = Dip.st.phy.Velocity.ExternalArm;

            Dip.ext.U = NR.CalculateOutput(InputDipNN)[0];

            if (std::abs(Dip.st.State[1]) >= (M_PI_4/4) || std::abs(Dip.st.State[2]) >= (M_PI_4/4)) 
            {
                Dip.ext.U =  NR.CalculateOutput(InputDipNN)[0];  
            } 
            else 
            {
                Dip.ext.U = -K[0]*Dip.st.State[0] -K[1]*Dip.st.State[1] -K[2]*Dip.st.State[2] -K[3]*Dip.st.State[3] -K[4]*Dip.st.State[4] -K[5]*Dip.st.State[5];
                // if (Dip.ext.U>15) Dip.ext.U=15;
                // if (Dip.ext.U<-15) Dip.ext.U=-15;
            }

            Dip_X = Dip.ComputeNextState(Ctx.GetStep(), &Dip);

            if(0 == i % 1000)
            {
                TimeZ = 0;
                Dip.ext.Z0 = 0;
            }
            if(TimeZ > 100) Dip.ext.Z0 = 0;
            TimeZ++;
            OutOfRange = false;
                

            DipHistory[0].push_back(Dip_X[0]);
            DipHistory[1].push_back(Dip_X[1]);
            DipHistory[2].push_back(Dip_X[2]);
            DipHistory[3].push_back(Dip_X[3]);
            DipHistory[4].push_back(Dip_X[4]);
            DipHistory[5].push_back(Dip_X[5]);
        }
        WriteToFile(DipHistory, "dip");
    }




    ///////////////// TIME CHECKING /////////////////
    auto StopSim = std::chrono::high_resolution_clock::now(); 
    auto TimeEvoT = std::chrono::duration_cast<std::chrono::seconds>(StartSim - StartEvo);
    auto TimeSimT = std::chrono::duration_cast<std::chrono::milliseconds>(StopSim - StartSim);
    auto TimeEvo = TimeEvoT.count();
    auto TimeSim = TimeSimT.count();
    cout << "Thank you for using N-Simulator. KamilAnd." << endl
     << "Evolution Time [s]: " << TimeEvo << endl
     << "Simulation Time [ms]: "<< TimeSim << endl;


    return 0;
}
