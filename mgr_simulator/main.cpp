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
#include "GlobalContext.hpp"
#include "AllModels.hpp"
#include "json/json.hpp"
#include "UsefulFunctions.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
using json = nlohmann::json;

GlobalContext Ctx;

int main() 
{
    /**
     * Reading JSON with configuration of simulation
     * 
    */
    json j;
    string RawJsonString = ReadFromFile("./json/simulation_context.json");
    j = json::parse(RawJsonString);


    /**
     * Preparing simulation context and parameters
     * 
    */
    // GlobalContext Ctx;
    Ctx.SetSimulationTimeSec(j["context"]["time"]);
    Ctx.SetProbesCountPerSec(j["context"]["probes_per_second"]);

    auto Now = chrono::system_clock::now();
    auto DateStamp = chrono::system_clock::to_time_t(Now);
    srand((unsigned int) DateStamp);


    /**
     * Declaration of elementary objects and variables
     * 
    */
    vector <unsigned int> _NodesCount = j["neural_network"]["nodes_count"];
    vector <ActivationFunction> _ActivationFunctions = j["neural_network"]["activation_functions"];
    NeuralNetwork NN(_NodesCount, _ActivationFunctions);
    
    NN.Randomization(j["neural_network"]["weights_range"], j["neural_network"]["biases_range"], 
                     j["neural_network"]["control_constant_range"]);

    NeuralRegulator NR(NN);


    /**
     * Execution of simulation and research
     * 
    */
    auto StartEvo = std::chrono::high_resolution_clock::now(); 

    EvolutionaryAlgorithm EVA(j["evo"]["population"], NN);
    for (int i = 0; i < j["evo"]["generation"]; i++)
    {
        NR = EVA.EvolveNextGeneration();
    }


    // Run simulation
    auto StartSim = std::chrono::high_resolution_clock::now(); 

    ///////////////// RLC /////////////////
    #define RLC_P j["series_rlc"]["parameters"]
    #define RLC_S j["series_rlc"]["init_state"]
    #define RLC_E j["series_rlc"]["external_forces"] // TODO

    if(j["onoff"]["series_rlc"])
    {
        SeriesRLC Rlc;
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
    #define DC_P j["dc"]["parameters"]
    #define DC_S j["dc"]["init_state"]
    #define DC_E j["dc"]["external_forces"]

    if(j["onoff"]["dc"])
    {
        DcMotor DcMotor;
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
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            Input[0] = (Setpoint - DcMotor.st.AngularVelocity);
            Input[1] = DcMotor.st.AngularVelocity + Noiser.Get();
            Input[2] = DcMotor.st.RotorCurrent + NoiseBig.Get();
            DcMotor.ext.U = NR.CalculateOutput(Input)[0];
            filtered = LPF.LowPass(DcMotor.st.RotorCurrent + NoiseBig.Get());

            if(i == 80){ DcMotor.ext.Tl = 15;}
            if(i == 190){ DcMotor.ext.Tl = 0;}
            if(i == 310){ DcMotor.ext.Tl = -7; }
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
    #define P_P j["pendulum"]["parameters"]
    #define P_S j["pendulum"]["init_state"]
    #define P_E j["pendulum"]["external_forces"]

    if(j["onoff"]["pendulum"])
    {
        Pendulum Pen;
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

            // Pen.ext.U = NR.CalculateOutput(Input)[0];
            Pen_X = Pen.ComputeNextState(Ctx.GetStep(), &Pen);

            PenHistory[0].push_back(Pen_X[0]);
            PenHistory[1].push_back(Pen_X[1]);
            PenHistory[2].push_back(Pen_X[2]);
            PenHistory[3].push_back(Pen_X[3]);
        }
        WriteToFile(PenHistory, "pen");
    }
    

    ///////////////// DOUBLE PENDULUM /////////////////
    #define DP_P j["double_pendulum"]["parameters"]
    #define DP_S j["double_pendulum"]["init_state"]
    #define DP_E j["double_pendulum"]["external_forces"]
    
    if(j["onoff"]["double_pendulum"])
    {
        DoublePendulum Dip;
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
        // double InputDipNN[] = {0, 0, 0, 0, 0, 0};
        
        for (int i = 0; i < Ctx.GetProbesCountTotal(); i++)
        {
            
            // InputDipNN[0] = Dip.st.phy.Position.Cart;
            // InputDipNN[1] = Dip.st.phy.Position.InnerArm;
            // InputDipNN[2] = Dip.st.phy.Position.ExternalArm;
            // InputDipNN[3] = Dip.st.phy.Velocity.Cart; 
            // InputDipNN[4] = Dip.st.phy.Velocity.InnerArm;
            // InputDipNN[5] = Dip.st.phy.Velocity.ExternalArm;

            // Dip.ext.U = NR.CalculateOutput(Input)[0];
            Dip_X = Dip.ComputeNextState(Ctx.GetStep(), &Dip);

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
